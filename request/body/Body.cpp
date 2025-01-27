/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/27 15:01:40 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"

Body::Body( void ) {
    this->bodyTrackingNumber = 0;
}

void Body::printBody( void ) {
    std::cout << this->body << std::endl;
}

Body::~Body() {}

void Body::setBody( std::string& body, int &trackingRequestNumber ) {
    (void) body;
    (void) trackingRequestNumber;
    Body::setChunkedBody(body, trackingRequestNumber);
}

void Body::setChunkedBody( std::string& body, int &trackingRequestNumber ) {
    (void)trackingRequestNumber;
    this->rest += body;
    if ( !this->bodyTrackingNumber  && this->rest.find("\r\n") != std::string::npos ) {
        this->bodyTrackingNumber = strtol(this->rest.substr(0, this->rest.find(";")).c_str(), NULL, 16);
        this->rest.erase(0, this->rest.find("\r\n") + 2);
    }
    if ( this->bodyTrackingNumber && this->rest.size() <= this->bodyTrackingNumber ) {
        // std::cout << "1 :" << this->bodyTrackingNumber << std::endl;
        // std::cout << "2 :" << this->rest << " <><><><> " << this->rest.size() << std::endl;
        this->body += this->rest;
        this->bodyTrackingNumber -= this->rest.size();
        this->rest = "";
    }
    else if ( this->bodyTrackingNumber ) {
        // std::cout << "3 :"  << this->bodyTrackingNumber << std::endl;
        // std::cout << "4 :"  << this->rest.size() << std::endl;
        this->body += this->rest.substr(0, this->bodyTrackingNumber);
        this->rest.erase(0, this->bodyTrackingNumber);
        // std::cout << this->rest << std::endl;
        this->bodyTrackingNumber = 0;
    }
    body = "";
}