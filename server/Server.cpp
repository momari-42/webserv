/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/01/20 19:23:06 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"
#define BUFFER_SIZE 4052
Server::Server ( std::vector<int> vec ) {
    this->lenSocket = sizeof(this->addressClient);
    for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++) {
        Socket soc(*it);
        this->sockets.push_back(soc);
    }
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        (*it).initializeSocketCommunication();
    }
    memset(&this->addressClient, 0, sizeof(this->addressClient));
}

bool Server::findFdSocket ( int sockfd ) {
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        if ((*it).getSockfd() == sockfd)
            return (true);
    }
    return (false);
}


// void Socket::socketAccepting ( void ) {
//     std::cout << "we are here" << std::endl;
//     close(this->sockfdClient);
//     this->sockfdClient = accept(this->sockfd, reinterpret_cast<sockaddr *>(&this->addressClient), &this->client_len);
//     if (this->sockfdClient == -1) {
//         std::cout << "Problem in accept function" << std::endl; 
//         throw (SocketExceptions(strerror(errno)));
//     }
// }

#include <fstream>


void Server::startServer() {
    int kq;
    int number = 0;
    int errorTrack;
    struct kevent  events[this->sockets.size()];

    
    memset(events, 0, sizeof(events));
    kq = kqueue();
    if ( kq == -1 )
        throw (Server::ServerExceptions(strerror(errno)));
    std::cout << "The fds sockets : " ;
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        EV_SET(&events[number], (*it).getSockfd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
        std::cout << (*it).getSockfd() << " - ";
        number++;
    }
    std::cout << std::endl;
    if (kevent(kq, events, this->sockets.size(), 0, 0, 0) == -1)
        throw (Server::ServerExceptions(strerror(errno)));
    while (true) {
        struct kevent  readyEvents[128];

        memset(readyEvents, 0, sizeof(readyEvents));
        
        errorTrack = kevent(kq, NULL, 0, readyEvents, 128, 0);
        if (errorTrack == -1)
            throw (Server::ServerExceptions(strerror(errno)));
        else if (errorTrack > 0) {
            for (int i = 0; i < errorTrack; i++) {
                int filedes = readyEvents[i].ident;
                if ( findFdSocket(filedes) ) {
                    struct kevent clientEvent;

                    this->sockfdClient = accept(filedes, reinterpret_cast<sockaddr *>(&this->addressClient), &this->lenSocket);
                    if (this->sockfdClient == -1) {
                        std::cout << "Problem in accept function" << std::endl; 
                        throw (ServerExceptions(strerror(errno)));
                    }

                    fcntl(filedes, O_NONBLOCK);
                    EV_SET(&clientEvent, this->sockfdClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                        throw (Server::ServerExceptions(strerror(errno)));

                }
                else  {

                // Handle data from a client socket
                    char buffer[BUFFER_SIZE];
                    ssize_t bytes_read = 0;
                    std::string request_data = "";

                    // Read all available data from the socket
                        memset(buffer, 0, BUFFER_SIZE);
                    while ((bytes_read = read(filedes, buffer, sizeof(buffer) - 1)) > 0) {
                        request_data += buffer;  // Accumulate the data
                        buffer[bytes_read] = '\0';

                        // Check if a complete request has been received
                        if (bytes_read < BUFFER_SIZE - 1) {
                            // We have a complete HTTP request
                            break;
                        }
                        memset(buffer, 0, BUFFER_SIZE);
                    }

                    if (bytes_read == 0) {
                        std::cout << "Client disconnected\n";
                        close(filedes);
                        continue;
                    } else if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("read error");
                        close(filedes);
                        continue;
                    }
                    size_t bytesRead = sizeof(buffer);
                    Request req(request_data);
                    std::string body = request_data.substr(request_data.find("\r\n\r\n") + 4);
                    while (bytesRead == sizeof(buffer)) {
                        bytesRead = recv(filedes, buffer, sizeof(buffer), 0);
                        std::cout << bytesRead << std::endl;
                        if (bytesRead <= 0) {
                            std::cerr << "Error or connection closed while reading data.\n";
                            close(filedes);
                            break;
                        }
                        body.append(buffer, bytesRead);
                    }
                    std::ofstream outputFile("hello.txt", std::ios::binary);
                    outputFile.write(body.data(), body.size());
                    outputFile.close();
                    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
                    write(filedes, response.c_str(), response.size());
                    std::cout << filedes << "......" << this->sockfdClient << std::endl;
                    close(filedes);  // Close the connection after sending the response
                }
            }
        }
        
    }
}

Server::~Server ( void ) {
}

Server::ServerExceptions::ServerExceptions ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

const char* Server::ServerExceptions::what() const throw() {
    return (this->errorMsg.c_str());
}

