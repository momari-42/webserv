/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/03/14 14:27:04 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

int endOfServer(std::string& content) {
    size_t index = 0;
    size_t count = 1;
    while ((unsigned long)index < content.length()) {
        if (content[index] == '}')
         count--;
        else if (content[index] == '{')
         count++;
        if (count == 0)
            return index;
        index++;
    }
    return -1;
}

Server::Server ( std::string& config ) {
    std::string line;
    std::string serverInfo;
    std::vector<std::string> parts;
    while (config.size()) {
        parts.clear();
        line = config.substr(0, config.find('\n'));
        config.erase(0, config.find('\n') + 1);
        ft_split(line, parts);
        if (!parts.size())
            continue;
        if (parts.size() != 2 || parts[0].compare("server") || parts[1].compare("{"))
            throw ErrorHandling("Syntax Error ");
        if (endOfServer(config) == -1)
            throw ErrorHandling("Syntax Error");
        serverInfo = config.substr(0, endOfServer(config));
        config.erase(0, endOfServer(config) + 2);
        ServerConfig server;
        server.parse(serverInfo);
        this->configs.push_back(server);
        server.showServerConfig();
    }
    checkServersConflict();
    
    // The old part
    this->lenSocket = sizeof(this->addressClient);
    for (std::vector<ServerConfig>::iterator it = this->configs.begin(); it != this->configs.end(); it++) {
        std::vector<std::pair<int, const std::string> > ports = (*it).getPorts();
        for (std::vector<std::pair<int, const std::string> >::iterator iter = ports.begin(); iter != ports.end(); iter++) {
            if (!checkSockets((*iter).second, (*iter).first, &(*it)))
                continue;
            Socket soc((*iter).first, (*iter).second, &(*it));
            // this->srvs[soc.getSockfd()] = *it;
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

bool Server::checkSockets(std::string host, int port, ServerConfig* server) {
    for (std::vector<Socket>::iterator it = this->sockets.begin(); it != this->sockets.end(); it++) {
        if ((*it).getHost() == host && (*it).getPort() == port) {
            (*it).setServer(server);
            return false;
        }
    }
    return true;
}

void Server::checkServersConflict() {
    for (std::vector<ServerConfig>::iterator it = this->configs.begin(); it < this->configs.end() - 1; it++)
    {
        for (std::vector<ServerConfig>::iterator iter = this->configs.begin() + 1; iter < this->configs.end(); iter++) {
            if (*it == *iter)
                throw ErrorHandling("Server Conflict Error");
        }
    }
}

bool Server::findFdSocket ( size_t sockfd ) {
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

    struct timespec time;
    time.tv_sec = 1;
    time.tv_nsec = 0;

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
    // for (std::map<int, ConfigFile>::iterator it = this->srvs.begin(); it != this->srvs.end(); it++) {
    //     std::cout << it->first << " - ";
    // }
    while (true) {
        memset(this->readyEvents, 0, sizeof(this->readyEvents));
        nevents = kevent(kq, NULL, 0, readyEvents, 128, &time);
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
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                        std::cout << "lolllllsdfdklsjfldjfldj" << std::endl;
                        throw (Server::ServerExceptions(strerror(errno)));
                    }
                    this->clients[this->sockfdClient].setConfig(this->readyFd, this->sockets);
                    this->timeout[this->sockfdClient] = std::time(NULL);
                }
                else if ( this->readyEvents[i].filter == EVFILT_READ ) {
                    this->timeout[this->readyFd] = std::time(NULL);
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
                        std::cout << "\033[1;31mthis is and error hap*****pend with code : " << this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() <<  "!\033[0m" << std::endl;
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() );
                        error.sendErrorPage();
                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                            std::cout << "lolllllsdfdklsjfldjf    1" << std::endl;
                            throw (Server::ServerExceptions(strerror(errno)));
                        }
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getRequest().getBodyComplete() == true) {
                        struct kevent clientEvent;

                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                            std::cout << "lolllllsdfdklsjfldjfldj   2" << std::endl;
                            throw (Server::ServerExceptions(strerror(errno)));
                        }

                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                            std::cout << "lolllllsdfdklsjfldjfldj   3" << std::endl;
                            throw (Server::ServerExceptions(strerror(errno)));
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_WRITE ) {
                    this->timeout[this->readyFd] = std::time(NULL);
                    this->clients[this->readyEvents[i].ident].getResponse().makeResponse( this->readyEvents[i].ident, kq );
                    if ( this->clients[this->readyEvents[i].ident].getResponse().getErrorCode().size() ) {                        
                        struct kevent clientEvent;

                        std::cout << "\033[1;31mthis is and error hap00pend with code : " << this->clients[this->readyEvents[i].ident].getResponse().getErrorCode() <<  "!\033[0m" << std::endl;
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getResponse().getErrorCode() );
                        error.sendErrorPage();
                        EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                            throw (Server::ServerExceptions(strerror(errno)));
                        }
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getResponse().getIsResponseSent()) {
                        if ( this->clients[this->readyEvents[i].ident].getRequest().getHeader()->getValue("Connection") == "close") {
                            std::cout << "\033[32mreqeust  closed : " << this->numberOfRequest++  << "\033[0m" << std::endl;
                            struct kevent clientEvent;

                            EV_SET(&clientEvent, this->readyEvents[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                                std::cout << "lolllllsdfdklsjfldjfldj       5" << std::endl;
                                throw (Server::ServerExceptions(strerror(errno)));
                            }
                            close(this->readyEvents[i].ident);
                            this->clients.erase(this->readyEvents[i].ident);
                        }
                        else {
                            std::cout << "\033[32mreqeust keep-alive : " << this->numberOfRequest++  << "\033[0m" << std::endl;
                            struct kevent clientEvent;

                            EV_SET(&clientEvent, this->readyFd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                                std::cout << "lolllllsdfdklsjfldjfldj    6" << std::endl;
                                throw (Server::ServerExceptions(strerror(errno)));
                            }
                            EV_SET(&clientEvent, this->readyFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                            if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                                std::cout << "lolllllsdfdklsjfldjfldj    7" << std::endl;
                                throw (Server::ServerExceptions(strerror(errno)));
                            }
                            this->clients[this->readyFd].getRequest().resetAttributes();
                            this->clients[this->readyFd].getResponse().resetAttributes();
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_PROC && (this->readyEvents[i].fflags & NOTE_EXIT)) {
                    struct kevent   clientEvent;
                    int             exitStatus = 0;
                    int             *fdClient  = static_cast<int *>(this->readyEvents[i].udata);

                    waitpid(this->readyEvents[i].ident, &exitStatus, WNOHANG);
                    EV_SET(&clientEvent, this->readyFd, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                        std::cout << "lolllllsdfdklsjfldjfldj    99" << std::endl;
                        throw (Server::ServerExceptions(strerror(errno)));
                    }
                    if (exitStatus) {
                        Error error( *fdClient, "500" );
                        error.sendErrorPage();
                        close(*fdClient);
                        this->clients.erase(*fdClient);
                    } else {
                        EV_SET(&clientEvent, *fdClient, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                        if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                            std::cout << "lolllllsdfdklsjfldjfldj" << std::endl;
                            throw (Server::ServerExceptions(strerror(errno)));
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_TIMER ) {
                    struct kevent   clientEvent;
                    int             *fdClient = static_cast<int *>(this->readyEvents[i].udata);

                    EV_SET(&clientEvent, this->readyFd, EVFILT_PROC, EV_DELETE, 0, 0, NULL);
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                        std::cout << "lolllllsdfdklsjfldjfldj     8 " << std::endl;
                        throw (Server::ServerExceptions(strerror(errno)));
                    }
                    EV_SET(&clientEvent, this->readyFd, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
                    if (kevent(kq, &clientEvent, 1, NULL, 0, NULL) == -1) {
                        std::cout << "lolllllsdfdklsjfldjfldj    9" << std::endl;
                        throw (Server::ServerExceptions(strerror(errno)));
                    }
                    kill(this->readyFd, SIGKILL);
                    Error error( *fdClient, "504" );
                    error.sendErrorPage();
                    close(*fdClient);
                    this->clients.erase(*fdClient);
                }
            }
        }
        for (std::map<size_t, size_t>::iterator it = this->timeout.begin(); it != this->timeout.end();) {
            if (std::time(NULL) - it->second > 30) {
                size_t fd = it->first;
                std::cout << "Client " << fd << " timed out" << std::endl;
                close(fd);
                this->clients.erase(fd);
                this->timeout.erase(fd);
                it = this->timeout.begin();
            } else {
                it++;
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
