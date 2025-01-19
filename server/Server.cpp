/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/01/19 10:42:59 by momari           ###   ########.fr       */
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


// void Socket::socketAccepting ( void ) {
//     std::cout << "we are here" << std::endl;
//     close(this->sockfdClient);
//     this->sockfdClient = accept(this->sockfd, reinterpret_cast<sockaddr *>(&this->addressClient), &this->client_len);
//     if (this->sockfdClient == -1) {
//         std::cout << "Problem in accept function" << std::endl; 
//         throw (SocketExceptions(strerror(errno)));
//     }
// }


void Server::startServer() {
    int kq;
    int number = 0;
    int errorTrack;
    struct kevent  events[this->sockets.size()];

    
    memset(events, 0, sizeof(events));
    kq = kqueue();
    if ( kq == -1 )
        throw (Server::ServerExceptions(strerror(errno)));
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        EV_SET(&events[number], (*it).getSockfd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
        number++;
    }
    if (kevent(kq, events, this->sockets.size(), 0, 0, 0) == -1)
        throw (Server::ServerExceptions(strerror(errno)));
    while (true) {
        struct kevent  readyEvents[128];
        memset(readyEvents, 0, sizeof(readyEvents));
        
        // std::cout << "****************************************************>>" <<  std::endl;
        errorTrack = kevent(kq, NULL, 0, readyEvents, 128, 0);
        // std::cout << "====================================================>>" << errorTrack <<  std::endl;
        if (errorTrack == -1)
            throw (Server::ServerExceptions(strerror(errno)));
        else if (errorTrack > 0) {
            for (int i = 0; i < errorTrack; i++) {
                if (readyEvents[i].filter == EVFILT_READ) {
                    if ( findFdSocket(readyEvents[i].ident) ) {
                        std::cout << "ila l9ahaaaaaaa" <<  std::endl;
                        struct kevent clientEvent;
                        this->sockfdClient = accept(readyEvents[i].ident, reinterpret_cast<sockaddr *>(&this->addressClient), &this->lenSocket);
                        std::cout << readyEvents[i].ident << "---->" << this->sockfdClient << std::endl;
                        if (this->sockfdClient == -1) {
                            std::cout << "Problem in accept function" << std::endl; 
                            throw (ServerExceptions(strerror(errno)));
                        }
                        fcntl(this->sockfdClient, O_NONBLOCK);
                        EV_SET(&clientEvent, this->sockfdClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                            throw (Server::ServerExceptions(strerror(errno)));
                    } else  {
                        std::cout << "ila mal9ahachchchchchchchchchchchchchchchch" <<  std::endl;
                        char buffer[800];
                        int re = recv(readyEvents[i].ident, buffer, 799, 0);
                        if (re <= 0) {
                            std::cout << "looooool" << std::endl;
                            break;
                        }
                        buffer[re] = '\0';
                        std::cout << buffer << std::endl;
                        memset(buffer, 0, 800);
                        struct kevent toDelete;
                        EV_SET(&toDelete, readyEvents[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        if ((kevent(kq, &toDelete, 1, NULL, 0, NULL)) == -1) {
                            throw (Server::ServerExceptions(strerror(errno)));
                        }
                        close(this->sockfdClient);
                        close(readyEvents[i].ident);
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

