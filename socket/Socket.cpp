/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/01/18 19:44:32 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Socket.hpp"
#include <fstream>

void Socket::setSockOption (void) {
    int option; 
    
    option = setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &this->addressServer, sizeof(this->addressServer));
    if (option == -1) {
        throw (SocketExceptions(strerror(errno)));
    }
}

Socket::Socket ( int port ) {
    // close(this->sockfd);
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        std::cout << "Problem in socket function" << std::endl;
        throw (SocketExceptions(strerror(errno)));
    }

    this->backlog = 5;

    // Init client and server struct with 0;
    memset(&this->addressServer, 0, sizeof(this->addressServer));

    // Specifies that the socket will use IPv4 addresses
    this->addressServer.sin_family = AF_INET;
    // Specifies the port number
    this->addressServer.sin_port = htons(port);
    // Specific IP address in this case htonl(INADDR_LOOPBACK) = 127.0.0.1
    this->addressServer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    Socket::setSockOption();

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

int Socket::getSockfd ( void ) {
    return (this->sockfd);
}

void Socket::initializeSocketCommunication ( void ) {
    Socket::socketBinding() ;
    Socket::socketListning();
}



Socket::SocketExceptions::SocketExceptions ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

const char* Socket::SocketExceptions::what() const throw() {
    return (this->errorMsg.c_str());
}

