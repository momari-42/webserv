/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/03/18 10:52:59 by zaelarb          ###   ########.fr       */
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
        // server.showServerConfig();
    }
    checkServersConflict();
    
    // The old part
    this->lenSocket = sizeof(this->addressClient);
    for (std::vector<ServerConfig>::iterator it = this->configs.begin(); it != this->configs.end(); it++) {
        std::vector<std::pair<const std::string, const std::string> > ports = (*it).getPorts();
        for (std::vector<std::pair<const std::string, const std::string> >::iterator iter = ports.begin(); iter != ports.end(); iter++) {
            if (!checkSockets((*iter).second, (*iter).first, &(*it)))
                continue;
            Socket soc((*iter).first, (*iter).second, &(*it));
            this->sockets.push_back(soc);
        }
    }

    memset(&this->addressClient, 0, sizeof(this->addressClient));
    // this just for delete
    this->numberOfRequest = 0;
}

bool Server::checkSockets(std::string host, std::string port, ServerConfig* server) {
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

void addFdToKqueue(int kq, size_t fd, int flag) {
    struct kevent change;
    
    EV_SET(&change, fd, flag, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        throw ErrorHandling(strerror(errno));
    }
}

void removeFdFromKqueue(int kq, size_t fd, int flag) {
    struct kevent change;
    
    EV_SET(&change, fd, flag, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        throw ErrorHandling(strerror(errno));
    }
}

void Server::startServer() {
    struct kevent   events[this->sockets.size()];
    char            buffer[BUFFER_SIZE];
    ssize_t         nevents;
    struct timespec time;


    //----------------------------------------------//
    //----------------------------------------------//



    std::ofstream of("momari.py", std::ios::binary);
    if (!of.is_open()) {
        std::cerr << "Error" << std::endl;
        exit(1);
    }
    
    //----------------------------------------------//
    //----------------------------------------------//

    time.tv_sec = 1;
    time.tv_nsec = 0;

    kq = kqueue();
    if ( kq == -1 )
        throw (Server::ServerExceptions(strerror(errno)));
    memset(events, 0, sizeof(events));
    for (size_t i = 0; i < this->sockets.size(); i++) {
        addFdToKqueue(kq, this->sockets[i].getSockfd(), EVFILT_READ);
    }
    
    while (true) {
        memset(this->readyEvents, 0, sizeof(this->readyEvents));
        nevents = kevent(kq, NULL, 0, readyEvents, 128, &time);
        if (nevents == -1)
            throw (Server::ServerExceptions(strerror(errno)));
        else if (nevents > 0) {
            for (ssize_t i = 0; i < nevents; i++) {
                this->readyFd = readyEvents[i].ident;
                if ( findFdSocket(this->readyFd) ) {
                    this->sockfdClient = accept(this->readyFd, reinterpret_cast<sockaddr *>(&this->addressClient), &this->lenSocket);

                    if (this->sockfdClient == -1) {
                        std::cout << "Problem in accept function" << std::endl; 
                        continue;
                    }
                    
                    fcntl(this->sockfdClient, F_SETFL, O_NONBLOCK);
                    addFdToKqueue(kq, this->sockfdClient, EVFILT_READ);
                    
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
                        std::cout << "\033[1;31mthis is and error hap*****pend with code : " << this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() <<  "!\033[0m" << std::endl;
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() );
                        error.sendErrorPage();
                        removeFdFromKqueue(kq, this->readyEvents[i].ident, EVFILT_READ);
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getRequest().getBodyComplete() == true) {
                        addFdToKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE);
                        removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_READ);
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_WRITE ) {
                    this->timeout[this->readyFd] = std::time(NULL);
                    this->clients[this->readyEvents[i].ident].getResponse().makeResponse( this->readyEvents[i].ident, kq );
                    if ( this->clients[this->readyEvents[i].ident].getResponse().getErrorCode().size() ) {                        
                        std::cout << "\033[1;31mthis is and error hap00pend with code : " << this->clients[this->readyEvents[i].ident].getResponse().getErrorCode() <<  "!\033[0m" << std::endl;
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getResponse().getErrorCode() );
                        error.sendErrorPage();
                        removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE);
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getResponse().getIsResponseSent()) {
                        if ( this->clients[this->readyEvents[i].ident].getRequest().getHeader()->getValue("CONNECTION") == "close") {
                            std::cout << "\033[32mreqeust  closed : " << this->numberOfRequest++  << "\033[0m" << std::endl;
                            removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE);
                            close(this->readyEvents[i].ident);
                            this->clients.erase(this->readyEvents[i].ident);
                        }
                        else {
                            std::cout << "\033[32mreqeust keep-alive : " << this->numberOfRequest++  << "\033[0m" << std::endl;
                            removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE);
                            addFdToKqueue(this->kq, this->readyEvents[i].ident, EVFILT_READ);
                            this->clients[this->readyFd].getRequest().resetAttributes();
                            this->clients[this->readyFd].getResponse().resetAttributes();
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_PROC && (this->readyEvents[i].fflags & NOTE_EXIT)) {
                    int             exitStatus = 0;
                    int             *fdClient  = static_cast<int *>(this->readyEvents[i].udata);

                    waitpid(this->readyEvents[i].ident, &exitStatus, WNOHANG);
                    removeFdFromKqueue(this->kq, this->readyFd, EVFILT_TIMER);
                    if (exitStatus) {
                        std::cout << "lolllllsdfdklsjfldjfldj    99 : " << WEXITSTATUS(exitStatus)  << std::endl;
                        Error error( *fdClient, "500" );
                        error.sendErrorPage();
                        close(*fdClient);
                        this->clients.erase(*fdClient);
                    } else
                        addFdToKqueue(this->kq, *fdClient, EVFILT_WRITE);
                }
                else if ( this->readyEvents[i].filter == EVFILT_TIMER ) {
                    int             *fdClient = static_cast<int *>(this->readyEvents[i].udata);
                    
                    removeFdFromKqueue(this->kq, this->readyFd, EVFILT_PROC);
                    removeFdFromKqueue(this->kq, this->readyFd, EVFILT_TIMER);
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
