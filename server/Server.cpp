/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/02/19 16:40:14 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

Server::Server ( std::vector<ConfigFile> vec ) {
    this->lenSocket = sizeof(this->addressClient);
    for (std::vector<ConfigFile>::iterator it = vec.begin(); it != vec.end(); it++) {
        std::map<int, std::string> ports = (*it).getPorts();
        for (std::map<int, std::string>::iterator iter = ports.begin(); iter != ports.end(); iter++) {
            Socket soc((*iter).first);
            this->srvs[soc.getSockfd()] = *it;
            this->sockets.push_back(soc);
        }
    }
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        (*it).initializeSocketCommunication();
    }
    memset(&this->addressClient, 0, sizeof(this->addressClient));
    // this just for delete
    this->numberOfRequest = 0;
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
                if ( findFdSocket(this->readyFd) ) {
                    struct kevent clientEvent;

                    fcntl(this->readyFd, F_SETFL, O_NONBLOCK);
                    this->sockfdClient = accept(this->readyFd, reinterpret_cast<sockaddr *>(&this->addressClient), &this->lenSocket);
                    if (this->sockfdClient == -1) {
                        std::cout << "Problem in accept function" << std::endl; 
                        throw (ServerExceptions(strerror(errno)));
                    }
                    EV_SET(&clientEvent, this->sockfdClient, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                        throw (Server::ServerExceptions(strerror(errno)));
                }
                else if ( this->readyEvents[i].filter == EVFILT_READ ) {
                    this->bytesRead = recv(this->readyFd, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead <= 0) {
                        std::cout << "Client disconnected" << std::endl;
                        close(this->readyFd);
                        this->clients.erase(this->readyFd);
                        continue;
                    }
                    buffer[this->bytesRead] = '\0';
                    of.write(buffer, bytesRead);
                    this->buffer.append(buffer, this->bytesRead);
                    this->clients[this->readyFd].getRequest().parseRequest(this->buffer);
                    memset(buffer, 0, BUFFER_SIZE);
                    this->buffer = "";
                    if ( this->clients[this->readyEvents[i].ident].getRequest().getErrorCode().size() ) {                        
                        struct kevent clientEvent;

                        std::cout << "\033[1;31mthis is and error happend with code : " << this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() <<  "!\033[0m" << std::endl;
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() );
                        error.sendErrorPage();
                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                            throw (Server::ServerExceptions(strerror(errno)));
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getRequest().getBodyComplete() == true) {
                        struct kevent clientEvent;

                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                            throw (Server::ServerExceptions(strerror(errno)));

                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                            throw (Server::ServerExceptions(strerror(errno)));
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_WRITE ) {
                    this->clients[this->readyEvents[i].ident].getResponse().makeResponse( this->readyEvents[i].ident );
                    if (this->clients[this->readyEvents[i].ident].getResponse().getIsResponseSent()) {
                        if ( this->clients[this->readyEvents[i].ident].getRequest().getHeader()->getValue("Connection") == "close") {
                            std::cout << "\033[32mreqeust  closed : " << this->numberOfRequest++  << "\033[0m" << std::endl;
                            struct kevent clientEvent;

                            EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                                throw (Server::ServerExceptions(strerror(errno)));
                            close(this->readyEvents[i].ident);
                            this->clients.erase(this->readyEvents[i].ident);
                        }
                        else {
                            std::cout << "\033[32mreqeust keep-alive : " << this->numberOfRequest++  << "\033[0m" << std::endl;
                            struct kevent clientEvent;

                            EV_SET(&clientEvent, this->readyFd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                                throw (Server::ServerExceptions(strerror(errno)));
                            EV_SET(&clientEvent, this->readyFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                            if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1)
                                throw (Server::ServerExceptions(strerror(errno)));
                            this->clients[this->readyFd].getRequest().resetAttributes();
                            this->clients[this->readyFd].getResponse().resetAttributes();
                        }
                    }
                }
                // else if ( this->readyEvents[i].filter == EVFILT_WRITE ) {
                    
                // }
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
