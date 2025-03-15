/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/03/15 11:23:12 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Socket.hpp"
#include <fstream>

void Socket::setServer(ServerConfig *server) {
    std::cout << "Push One Server" << std::endl;
    this->servers.push_back(server);
}
std::string& Socket::getPort() {
    return this->port;
}
std::string& Socket::getHost() {
    return this->host;
}

void Socket::setSockOption (void) {
    int option; 
    
    option = setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &this->addressServer, sizeof(this->addressServer));
    if (option == -1) {
        throw (SocketExceptions(strerror(errno)));
    }
    // std::cout << "looooool" << std::endl;

}

ServerConfig* Socket::getServerConfig(std::string serverName) {
    // std::cout << "get Server config " << std::endl;
    // std::cout << "Hi " << this->servers.size() << std::endl;
    // for (std::vector<ServerConfig*>::iterator it = this->servers.begin(); it != this->servers.end(); it++) {
    //     // std::cout << "loool" << std::endl;
    //     if ((*it)->isExistName(serverName)) {
    //         // std::cout << "loool222222" << std::endl;
    //         return *it;
            
    //     }
    // }
    std::cout << "Before all the things " << this->servers.size() << std::endl;
    for (size_t i = 0; i < this->servers.size(); i++) {
        std::cout << "This the server number : " << i << std::endl;
        if(this->servers[i]->isExistName(serverName))
            return (this->servers[i]);
    }
    return (this->servers[0]);
}

Socket::Socket ( const std::string& port, const std::string& host , ServerConfig *server) {
    // close(this->sockfd);
    // (void) host;
    // this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if (this->sockfd == -1) {
    //     std::cout << "Problem in socket function" << std::endl;
    //     throw (SocketExceptions(strerror(errno)));
    // }
    // this->backlog = 5;

    // // Init client and server struct with 0;
    // memset(&this->addressServer, 0, sizeof(this->addressServer));

    // // Specifies that the socket will use IPv4 addresses
    // this->addressServer.sin_family = AF_INET;
    // // Specifies the port number
    // this->addressServer.sin_port = htons(port);
    // // Specific IP address in this case htonl(INADDR_LOOPBACK) = 127.0.0.1
    // this->addressServer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    struct addrinfo hints, *res;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
        throw ErrorHandling("Getaddrinfo Error: ");
    }

    this->sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (this->sockfd == -1) {
        std::cout << "Problem in socket function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }

    if ( setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, res->ai_addr, res->ai_addrlen) == -1) {
        throw (SocketExceptions(strerror(errno)));
    }
    
    if (bind(this->sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cout << "Problem in bind function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }

    freeaddrinfo(res);
    this->servers.push_back(server);
    this->host = host;
    this->port = port;
}

Socket::~Socket ( void ) {
}

void Socket::socketBinding ( void ) {
    int status;

    // The bind function link the socket file descriptor with a specific socket address (IP Adress and Port Number)
    status = bind(this->sockfd, reinterpret_cast<const sockaddr *>(&this->addressServer), sizeof(this->addressServer));
    if (status == -1) {
        std::cout << "Problem in bind function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }
}

void Socket::socketListning ( void ) {
    int status;

    status = listen(this->sockfd, this->backlog);
    if (status == -1) {
        std::cout << "Problem in listen function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }
}

size_t Socket::getSockfd ( void ) {
    return (this->sockfd);
}

void Socket::initializeSocketCommunication ( void ) {
    // Socket::socketBinding() ;
    Socket::socketListning();
}



Socket::SocketExceptions::SocketExceptions ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

const char* Socket::SocketExceptions::what() const throw() {
    return (this->errorMsg.c_str());
}

