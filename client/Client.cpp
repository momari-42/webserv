/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:09 by momari            #+#    #+#             */
/*   Updated: 2025/03/05 00:48:17 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : request(), response( &this->request, this->socket ) {
    
}

Client::~Client() {
    
}

void Client::setConfig(size_t fd, std::vector<Socket> sockets) {
    for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++) {
        if ((*it).getSockfd() == fd)
            this->socket = &(*it);
    }
}

Request &Client::getRequest() {
    return (this->request);
}

Response &Client::getResponse() {
    return (this->response);
}