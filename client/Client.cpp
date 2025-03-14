/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:09 by momari            #+#    #+#             */
/*   Updated: 2025/03/09 13:39:24 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : request(), response( &this->request ) {
    this->isConfigFileInitialized   = false;
}

Client::~Client() {

}

void Client::setConfig(size_t fd, std::vector<Socket>& sockets) {
    for (size_t i = 0; i < sockets.size() - 1; i++) {
        if (sockets[i].getSockfd() == fd) {
            this->socket = &(sockets[i]);
            this->request.setSocket(&(sockets[i]));
            this->response.setSocket(&(sockets[i]));
            break;
        }
    }
    this->name = "some how";
    // std::cout << this->socket->servers.size() << " HHHHHHHHHHH " << std::endl;
}

Request &Client::getRequest() {
    return (this->request);
}

Response &Client::getResponse() {
    return (this->response);
}