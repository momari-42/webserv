/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/03/22 15:47:54 by momari           ###   ########.fr       */
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

bool addFdToKqueue(int kq, size_t fd, int flag) {
    struct kevent change;
    
    EV_SET(&change, fd, flag, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        return (true);
    }
    return (false);
}

bool removeFdFromKqueue(int kq, size_t fd, int flag) {
    struct kevent change;
    
    EV_SET(&change, fd, flag, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        return (true);
    }
    return (false);
}

void Server::startServer() {
    struct kevent   events[this->sockets.size()];
    char            buffer[BUFFER_SIZE];
    ssize_t         nevents;
    struct timespec time;

    time.tv_sec = 1;
    time.tv_nsec = 0;

    kq = kqueue();
    if ( kq == -1 )
        throw (Server::ServerExceptions(strerror(errno)));
    memset(events, 0, sizeof(events));
    for (size_t i = 0; i < this->sockets.size(); i++) {
        if ( addFdToKqueue(kq, this->sockets[i].getSockfd(), EVFILT_READ))
            throw ErrorHandling(strerror(errno));
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
                        std::cerr << "Problem in accept function" << std::endl; 
                        continue;
                    }
                    fcntl(this->sockfdClient, F_SETFL, O_NONBLOCK);
                    if ( addFdToKqueue(kq, this->sockfdClient, EVFILT_READ)) {
                        std::map<std::string, std::string> errorPages;
                        Error error( this->sockfdClient, "500",  errorPages);
                        error.sendErrorPage();
                        std::cerr << "Problem in kevent (failed add Event of client to kq)" << std::endl; 
                        close(this->sockfdClient);
                        continue;
                    }
                    
                    this->clients[this->sockfdClient].setConfig(this->readyFd, this->sockets);
                    this->timeout[this->sockfdClient] = std::time(NULL);
                    std::cout << "\033[32mNew client Connected :)"   << "\033[0m" << std::endl;
                }
                else if ( this->readyEvents[i].filter == EVFILT_READ ) {
                    this->timeout[this->readyFd] = std::time(NULL);
                    this->bytesRead = recv(this->readyFd, buffer, sizeof(buffer) - 1, 0);
                    // std::cerr << "." << std::endl;
                    if (this->bytesRead <= 0) { 
                        if ( bytesRead == 0)
                            std::cerr << "  Client disconnected" << std::endl;
                        else {
                            std::map<std::string, std::string> errorPages;
                            Error error( this->sockfdClient, "500",  errorPages);
                            error.sendErrorPage();
                            std::cerr << "  recv failed !!" << std::endl;
                        }
                        close(this->readyFd);
                        this->clients.erase(this->readyFd);
                        this->timeout.erase(this->readyFd);
                        continue;
                    }
                    buffer[this->bytesRead] = '\0';
                    this->buffer.append(buffer, this->bytesRead);
                    this->clients[this->readyFd].getRequest().parseRequest(this->buffer);
                    memset(buffer, 0, BUFFER_SIZE);
                    this->buffer = "";
                    if ( this->clients[this->readyEvents[i].ident].getRequest().getErrorCode().size() ) {
                        std::cout << "\033[1;31m    this is and error happend with code : " << this->clients[this->readyEvents[i].ident].getRequest().getErrorCode() <<  "!\033[0m" << std::endl;

                        std::map<std::string, std::string> errorPages;
                        ServerConfig *configFile = this->clients[this->readyEvents[i].ident].getRequest().getConfigFile();
                        if (configFile)
                            errorPages = configFile->getErrorPages();
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getRequest().getErrorCode(), errorPages );
                        error.sendErrorPage();
                        if ( removeFdFromKqueue(kq, this->readyEvents[i].ident, EVFILT_READ) ) {
                            Error error( this->readyEvents[i].ident, "500", errorPages );
                            error.sendErrorPage();
                            std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl; 
                        }
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                        this->timeout.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getRequest().getBodyComplete() == true) {
                        if ( addFdToKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE) ) {
                            Error error( this->readyEvents[i].ident, "500", this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                            error.sendErrorPage();
                            std::cerr << "Problem in kevent (failed add Event of client to kq)" << std::endl; 
                            close(this->readyEvents[i].ident);
                            this->clients.erase(this->readyEvents[i].ident);
                            this->timeout.erase(this->readyEvents[i].ident);
                        }
                        if ( removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_READ) ) {
                            Error error( this->readyEvents[i].ident, "500", this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                            error.sendErrorPage();
                            std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl; 
                            close(this->readyEvents[i].ident);
                            this->clients.erase(this->readyEvents[i].ident);
                            this->timeout.erase(this->readyEvents[i].ident);
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_WRITE ) {
                    this->timeout[this->readyFd] = std::time(NULL);
                    this->clients[this->readyEvents[i].ident].getResponse().makeResponse( this->readyEvents[i].ident, kq );
                    if ( this->clients[this->readyEvents[i].ident].getResponse().getErrorCode().size() ) {                        
                        Error error( this->readyEvents[i].ident, this->clients[this->readyEvents[i].ident].getResponse().getErrorCode(), this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                        error.sendErrorPage();
                        std::cout << "\033[1;31m    this is and error happend with code : " << this->clients[this->readyEvents[i].ident].getResponse().getErrorCode() <<  "!\033[0m" << std::endl;
                        if ( removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE) ) {
                            Error error( this->readyEvents[i].ident, "500", this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                            error.sendErrorPage();
                            std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl; 
                        }
                        close(this->readyEvents[i].ident);
                        this->clients.erase(this->readyEvents[i].ident);
                        this->timeout.erase(this->readyEvents[i].ident);
                    }
                    else if (this->clients[this->readyEvents[i].ident].getResponse().getIsResponseSent()) {
                        if ( this->clients[this->readyEvents[i].ident].getRequest().getHeader()->getValue("CONNECTION") == "close") {
                            std::cout << "\033[32m  reqeust  closed : " << this->numberOfRequest++ << " Done!!"  << "\033[0m" << std::endl;
                            if (removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE)) {
                                Error error( this->readyEvents[i].ident, "500", this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                                error.sendErrorPage();
                                std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl; 
                            }
                            close(this->readyEvents[i].ident);
                            this->clients.erase(this->readyEvents[i].ident);
                            this->timeout.erase(this->readyEvents[i].ident);
                        }
                        else {
                            std::cout << "\033[32m  reqeust keep-alive : " << this->numberOfRequest++  << " Done!!"  << "\033[0m" << std::endl;
                            if (removeFdFromKqueue(this->kq, this->readyEvents[i].ident, EVFILT_WRITE)) {
                                Error error( this->readyEvents[i].ident, "500", this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                                error.sendErrorPage();
                                std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl;
                                close(this->readyEvents[i].ident);
                                this->clients.erase(this->readyEvents[i].ident);
                                this->timeout.erase(this->readyEvents[i].ident);
                                continue;
                            }
                            if ( addFdToKqueue(this->kq, this->readyEvents[i].ident, EVFILT_READ) ) {
                                Error error( this->readyEvents[i].ident, "500", this->clients[this->readyEvents[i].ident].getRequest().getConfigFile()->getErrorPages() );
                                error.sendErrorPage();
                                std::cerr << "Problem in kevent (failed add Event of client to kq)" << std::endl;
                                close(this->readyEvents[i].ident);
                                this->clients.erase(this->readyEvents[i].ident);
                                this->timeout.erase(this->readyEvents[i].ident);
                                continue;
                            }
                            this->clients[this->readyFd].getResponse().resetAttributes();
                            this->clients[this->readyFd].getRequest().resetAttributes();
                            this->clients[this->readyFd].getResponse().setIsReadyForNextRequest(true);
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_PROC && (this->readyEvents[i].fflags & NOTE_EXIT)) {
                    int             exitStatus = 0;
                    int             fdClient  = *(static_cast<int *>(this->readyEvents[i].udata));

                    waitpid(this->readyEvents[i].ident, &exitStatus, WNOHANG);
                    if (removeFdFromKqueue(this->kq, this->readyFd, EVFILT_TIMER)) {
                        Error error( fdClient, "500", this->clients[fdClient].getRequest().getConfigFile()->getErrorPages() );
                        error.sendErrorPage();
                        std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl;
                        close(fdClient);
                        this->clients.erase(fdClient);
                        this->timeout.erase(fdClient);
                        continue;
                    }
                    if (WEXITSTATUS(exitStatus)) {
                        Error error( fdClient, "500", this->clients[fdClient].getRequest().getConfigFile()->getErrorPages() );
                        error.sendErrorPage();
                        close(fdClient);
                        this->clients.erase(fdClient);
                        this->timeout.erase(fdClient);
                    } else {
                        if (addFdToKqueue(this->kq, fdClient, EVFILT_WRITE)) {
                            Error error( fdClient, "500", this->clients[fdClient].getRequest().getConfigFile()->getErrorPages() );
                            error.sendErrorPage();
                            close(fdClient);
                            this->clients.erase(fdClient);
                            this->timeout.erase(fdClient);
                        }
                    }
                }
                else if ( this->readyEvents[i].filter == EVFILT_TIMER ) {
                    int             fdClient = *(static_cast<int *>(this->readyEvents[i].udata));
                    
                    kill(this->readyFd, SIGKILL);
                    Error error( fdClient, "408", this->clients[fdClient].getRequest().getConfigFile()->getErrorPages() );
                    error.sendErrorPage();
                    close(fdClient);
                    this->clients.erase(fdClient);
                    this->timeout.erase(fdClient);
                    if ( removeFdFromKqueue(this->kq, this->readyFd, EVFILT_PROC)) {
                        std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl;
                        close(fdClient);
                        this->clients.erase(fdClient);
                        this->timeout.erase(fdClient);
                    }
                    if (removeFdFromKqueue(this->kq, this->readyFd, EVFILT_TIMER)) {
                        std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl;
                        close(fdClient);
                        this->clients.erase(fdClient);
                        this->timeout.erase(fdClient);
                    }
                }
            }
        }
        for ( std::map<size_t, Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
            if ( !it->second.getRequest().getBody()->getNextRequest().empty() && it->second.getIsReadyForNextRequest() ) {
                it->second.getRequest().parseRequest(it->second.getRequest().getBody()->getNextRequest());
                it->second.getRequest().getBody()->setNextRequest("");
                if ( it->second.getRequest().getErrorCode().size() ) {
                    std::cout << "\033[1;31mthis is and error happend with code : " <<it->second.getRequest().getErrorCode() <<  "!\033[0m" << std::endl;
                    Error error( it->first, it->second.getRequest().getErrorCode(), it->second.getRequest().getConfigFile()->getErrorPages() );
                    error.sendErrorPage();
                    if (removeFdFromKqueue(kq, it->first, EVFILT_READ))
                        std::cerr << "Problem in kevent (failed remove Event of client from kq)" << std::endl;
                    close(it->first);
                    this->clients.erase(it->first);
                    this->timeout.erase(it->first);
                }
                else if (it->second.getRequest().getBodyComplete() == true) {
                    if (addFdToKqueue(this->kq, it->first, EVFILT_WRITE)) {
                        close (it->first);
                        this->clients.erase(it->first);
                        this->timeout.erase(it->first);
                    }
                    if (removeFdFromKqueue(this->kq, it->first, EVFILT_READ)) {
                        close (it->first);
                        this->clients.erase(it->first);
                        this->timeout.erase(it->first);
                    }
                }
            }
        }
        for (std::map<size_t, size_t>::iterator it = this->timeout.begin(); it != this->timeout.end();) {
            if (std::time(NULL) - it->second > 30) {
                size_t fd = it->first;
                std::cout << "Client timed out" << std::endl;
                this->clients.erase(fd);
                this->timeout.erase(fd);
                close(fd);
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
