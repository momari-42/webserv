/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/26 12:58:02 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) {
    this->trackingRequestNumber = 0;
}

void Request::parseRequest ( std::string& requestData ) {
    std::cout << requestData;
    if (this->trackingRequestNumber == 0) {
        if (requestData.find("\r\n") != std::string::npos) {
            this->requestLine.setRequestLine(requestData.substr(0, requestData.find("\r\n")));
            requestData.erase(0, requestData.find("\r\n") + 2);
            this->trackingRequestNumber++;
        }
        else {
            //  error "414 Request-URI Too Large";
        }
        
    }

    if (this->trackingRequestNumber == 1) {
        if (requestData.find("\r\n\r\n") != std::string::npos) {
            this->header.setHeader(requestData.substr(0, requestData.find("\r\n\r\n") + 2));
            requestData.substr(0, requestData.find("\r\n\r\n") + 4);

            this->trackingRequestNumber++;
        } else {
            this->header.setHeader(requestData);
        }
    }
    if (this->trackingRequestNumber == 2) {
        
    }
}

Request::~Request () {
    
}