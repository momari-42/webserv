/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:09 by momari            #+#    #+#             */
/*   Updated: 2025/02/19 16:41:34 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) : request(), response( &this->request ) {
    
}

Client::~Client() {
    
}

Request &Client::getRequest() {
    return (this->request);
}

Response &Client::getResponse() {
    return (this->response);
}