/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:07:18 by momari            #+#    #+#             */
/*   Updated: 2025/01/11 11:49:52 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Server.hpp"

Server::Server (int port) {
    // close(this->sockfdServer);
    this->sockfdServer = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfdServer == -1) {
        std::cout << "Problem in socket function" << std::endl;
        throw (ServerExceptions(strerror(errno)));
    }

    this->client_len = sizeof(this->addressClient);
    this->backlog = 5;
    // Init client and server struct with 0;
    memset(&this->addressServer, 0, sizeof(this->addressServer));
    memset(&this->addressClient, 0, sizeof(this->addressClient));

    // Specifies that the socket will use IPv4 addresses
    this->addressServer.sin_family = AF_INET;
    // Specifies the port number
    this->addressServer.sin_port = htons(port);
    // Specific IP address in this case htonl(INADDR_LOOPBACK) = 127.0.0.1
    this->addressServer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int opt; 
    opt = setsockopt(this->sockfdServer, SOL_SOCKET, SO_REUSEADDR, &this->addressServer, sizeof(this->addressServer));
}

Server::~Server ( void ) {
    close(this->sockfdServer);
}

void Server::serverBinding ( void ) {
    int status;

    // The bind function link the socket file descriptor with a specific socket address (IP Adress and Port Number)
    status = bind(this->sockfdServer, reinterpret_cast<const sockaddr *>(&this->addressServer), sizeof(this->addressServer));
    if (status == -1) {
        std::cout << "Problem in bind function" << std::endl;
        throw (ServerExceptions(strerror(errno)));
    }
}

void Server::serverListning ( void ) {
    int status;

    status = listen(this->sockfdServer, this->backlog);
    if (status == -1) {
        std::cout << "Problem in listen function" << std::endl;
        throw (ServerExceptions(strerror(errno)));
    }
}

void Server::serverAccepting ( void ) {
    int flag = 0;
    close(this->sockfdClient);
    while (1) {
        this->sockfdClient = accept(this->sockfdServer, reinterpret_cast<sockaddr *>(&this->addressClient), &this->client_len);
        if (this->sockfdClient == -1) {
            std::cout << "Problem in accept function" << std::endl;
            throw (ServerExceptions(strerror(errno)));
        }
        Server::receiveRequest();
        // here we will create the requiste object
        std::cout << this->request << std::endl;
        std::cout << "this is the request number : ==> " << flag++  << std::endl;
        this->request = "";
    }
}

void Server::receiveRequest ( void ) {
    int     n;
    char    buffer[11];

    n = 10;
    memset(buffer, 0, 11);
    while (n >= 10) {
        n = recv(this->sockfdClient, buffer, 10, 0);
        buffer[n] = '\0';
        this->request += buffer;
        memset(buffer, 0, 11);
    }
}
void Server::runServer ( void ) {
    Server::serverBinding();
    Server::serverListning();
    Server::serverAccepting();
}

Server::ServerExceptions::ServerExceptions ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

const char* Server::ServerExceptions::what() const throw() {
    return (this->errorMsg.c_str());
}

