/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/03/23 00:40:27 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Socket.hpp"
#include <fstream>

void Socket::setServer(ServerConfig *server) {
    this->servers.push_back(server);
}

ServerConfig* Socket::getServerConfig(std::string serverName) {
    for (size_t i = 0; i < this->servers.size(); i++) {
        if(this->servers[i]->isExistName(serverName))
            return (this->servers[i]);
    }
    return (this->servers[0]);
}

Socket::Socket ( const std::string& port, const std::string& host , ServerConfig *server) {
    struct addrinfo hints, *res;
    
    this->backlog = 10;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
        throw ErrorHandling("Getaddrinfo Error: ");
    }

    this->sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (this->sockfd == -1) {
        std::cerr << "Problem in socket function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }

    if ( setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, res->ai_addr, res->ai_addrlen) == -1) {
        throw (SocketExceptions(strerror(errno)));
    }
    
    fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
    
    if (bind(this->sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Problem in bind function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }

    if (listen(this->sockfd, this->backlog) == -1) {
        std::cerr << "Problem in listen function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }

    freeaddrinfo(res);
    this->servers.push_back(server);
    this->host = host;
    this->port = port;
    std::cout << "      The server listening on " << this->host << ":" << this->port << "..." << std::endl;
}

Socket::~Socket ( void ) {
}

size_t Socket::getSockfd ( void ) {
    return (this->sockfd);
}

std::string& Socket::getPort() {
    return this->port;
}

std::string& Socket::getHost() {
    return this->host;
}

Socket::SocketExceptions::SocketExceptions ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

const char* Socket::SocketExceptions::what() const throw() {
    return (this->errorMsg.c_str());
}