/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/30 17:37:20 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) : header(), body(&header) {
    this->trackingRequestNumber = 0;
}

// Red     : \033[31m
// Green   : \033[32m
// Yellow  : \033[33m
// Blue    : \033[34m
// Magenta : \033[35m
// Cyan    : \033[36m
// White   : \033[37m

void Request::parseRequest ( std::string requestData ) {
    if (this->trackingRequestNumber == 0) {
        this->requestLine.setRequestLine(requestData, this->trackingRequestNumber);
    }
    if (this->trackingRequestNumber == 1) {
        this->header.setHeader(requestData, this->trackingRequestNumber);
    }
    if (this->trackingRequestNumber == 2) {
        this->body.setBody( requestData );
    }
}


bool Request::getBodyComplete( void) {
    return (this->body.getBodyComplete());
}


void Request::print( void ) {
    // std::cout << "\033[34m" << "-----------------------------------------------" << "\0303[m" << std::endl;
    // std::cout << "\033[34m" << "|---------This is the Requesrt Line-----------|" << "\033[0m" << std::endl;
    // std::cout << "\033[34m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    // this->requestLine.printFirstLine();
    // std::cout << "\033[32m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    // std::cout << "\033[32m" << "|--------------This is the Header-------------|" << "\033[0m" << std::endl;
    // std::cout << "\033[32m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    // this->header.print();
    std::cout << "\033[31m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "|--------------This is the Body---------------|" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    this->body.printBody();
    
}


Request::~Request () {
    
}