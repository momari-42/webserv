/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/02/08 11:46:22 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

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

void Server::startServer() {
    int             number = 0;
    struct kevent   events[this->sockets.size()];
    char            buffer[BUFFER_SIZE];
    ssize_t         nevents;
    



    //----------------------------------------------//
    //----------------------------------------------//



    std::ofstream of("momari.py", std::ios::binary);
    if (!of.is_open()) {
        std::cerr << "Error" << std::endl;
        exit(1);
    }

    
    //----------------------------------------------//
    //----------------------------------------------//
    
    kq = kqueue();
    if ( kq == -1 )
        throw (Server::ServerExceptions(strerror(errno)));
    memset(events, 0, sizeof(events));
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        EV_SET(&events[number], (*it).getSockfd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
        number++;
    }
    if (kevent(kq, events, this->sockets.size(), 0, 0, 0) == -1)
        throw (Server::ServerExceptions(strerror(errno)));
    while (true) {

        memset(this->readyEvents, 0, sizeof(this->readyEvents));
        
        nevents = kevent(kq, NULL, 0, readyEvents, 128, 0);
        if (nevents == -1)
            throw (Server::ServerExceptions(strerror(errno)));
        else if (nevents > 0) {
            for (ssize_t i = 0; i < nevents; i++) {
                this->readyFd = readyEvents[i].ident;
                fcntl(this->readyFd, F_SETFL, O_NONBLOCK);
                if ( findFdSocket(this->readyFd) ) {
                    struct kevent clientEvent;
                    this->sockfdClient = accept(this->readyFd, reinterpret_cast<sockaddr *>(&this->addressClient), &this->lenSocket);
                    if (this->sockfdClient == -1) {
                        std::cout << "Problem in accept function" << std::endl; 
                        throw (ServerExceptions(strerror(errno)));
                    }
                    EV_SET(&clientEvent, this->sockfdClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                        throw (Server::ServerExceptions(strerror(errno)));
                }
                else  {
                    this->bytesRead = recv(this->readyFd, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead <= 0)
                        break;
                    buffer[this->bytesRead] = '\0';
                    of.write(buffer, bytesRead);
                    this->buffer.append(buffer, this->bytesRead);
                    requestsClient[this->readyFd].parseRequest(this->buffer);
                    memset(buffer, 0, BUFFER_SIZE);
                    this->buffer = "";
                    
                    // std::cout  << "\033[32m" << bytesRead << "\033[0m" << std::endl;

                    // std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
                    // write(this->readyFd, response.c_str(), response.size());
                    // close(this->readyFd);  // Close the connection after sending the response
                    if (this->requestsClient[this->readyFd].getBodyComplete() == true) {
                        // this is where i call the response methode
                        // std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
                        // write(this->readyFd, response.c_str(), response.size());
                        // Error  error(this->readyFd, "404");
                        // std::cout << "we are here" << std::endl;
                        Response resp(this->readyFd, &this->requestsClient[this->readyFd]);
                        // res.sendErrorPage();
                        resp.makeResponse();
                        close(this->readyFd);  // Close the connection after sending the response
                        requestsClient.erase(this->readyFd);
    
                    }
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




// we get the content length() ;;;

// if (request_data.find("\r\n\r\n") != std::string::npos) {
//     if (request_data.find(contentString) != std::string::npos) {
//         long startContentLength = request_data.find(contentString) + contentString.length();
//         long endContentLength = request_data.find("\r\n", startContentLength);
//         long contentLength = std::stod(request_data.substr(startContentLength, endContentLength - startContentLength));
//         if (request_data.length() >= contentLength + 4 + request_data.find("\r\n\r\n")) {
//             std::cout  << "\033[32m" << "body received success !!!" << "\033[0m" << std::endl;
//             break;
//         }

//         // std::cout << "\033[33m" << request_data.length() << "<=>" << contentLength + 4 + request_data.find("\r\n\r\n") << "\033[0m" << std::endl;

//     }
//     else {

//         std::cout << "\033[31m" << "body not found !!!" << "\033[0m" << std::endl;

//         break;
//     }
// }