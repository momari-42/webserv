/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:09 by momari            #+#    #+#             */
/*   Updated: 2025/03/23 02:43:03 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : request( isReadyForNextRequest ), response( &this->request, isReadyForNextRequest ) {
    this->isReadyForNextRequest     = false;
    this->isConfigFileInitialized   = false;
}

Client::~Client() {

}

void Client::setConfig(size_t fd, std::vector<Socket>& sockets) {
    for (size_t i = 0; i < sockets.size(); i++) {
        if (sockets[i].getSockfd() == fd) {
            this->socket = &(sockets[i]);
            this->request.setSocket(&(sockets[i]));
            this->response.setSocket(&(sockets[i]));
            break;
        }
    }
}

Request &Client::getRequest() {
    return (this->request);
}

Response &Client::getResponse() {
    return (this->response);
}

bool Client::getIsReadyForNextRequest() {
    return (this->isReadyForNextRequest);
}