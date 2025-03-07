/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:09 by momari            #+#    #+#             */
/*   Updated: 2025/03/07 08:43:18 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : request(), response( &this->request ) {
    this->isConfigFileInitialized   = false;
}

Client::~Client() {

}

void Client::setConfig(size_t fd, std::vector<Socket>& sockets) {
    std::cout << "From set config " << sockets[0].servers.size() << std::endl;
    // for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++) {
    //     if ((*it).getSockfd() == fd) {
    //         this->socket = &(*it);
    //         break;
    //     }
    // }
    for (size_t i = 0; i < sockets.size() - 1; i++) {
        std::cout << i << " - fd sock " << sockets[i].getSockfd() << " fd param " << fd << std::endl;
        if (sockets[i].getSockfd() == fd) {
            this->socket = &(sockets[i]);
            break;
        }
    }
    this->name = "some how";
    std::cout << this->socket->servers.size() << " HHHHHHHHHHH " << std::endl;
}

Request &Client::getRequest() {
    if (this->request.getTrackingRequestNumber() == 2 && !this->isConfigFileInitialized) {
        std::cout << "The name of clieant is " << this->name << std::endl;
        this->request.setConfigFile(this->socket->getServerConfig(this->request.getHeader()->getValue("Host")));
        this->request.getBody()->setConfigFile(this->socket->getServerConfig(this->request.getHeader()->getValue("Host")));
        this->response.setConfigFile(this->socket->getServerConfig(this->request.getHeader()->getValue("Host")));
        this->isConfigFileInitialized = true;
    }
    return (this->request);
}

Response &Client::getResponse() {
    return (this->response);
}