/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:12:25 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/27 09:18:13 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"

RequestLine::RequestLine( void ) {
}


void RequestLine::printFirstLine( void ) {
    std::cout << "1;) " << this->method << std::endl;
    std::cout << "1;) " << this->requestTarget << std::endl;
    std::cout << "1;) " << this->httpVersion << std::endl;
}

void RequestLine::setRequestLine( std::string& requestLine, int& trackingRequestNumber ) {
    this->rest += requestLine;
    // std::cout << "a : " << this->rest << std::endl;
    if (this->rest.find("\r\n") != std::string::npos) {
        this->rest.erase(this->rest.find("\r\n"));
        if (this->rest.find('\t') != std::string::npos || this->rest.find_first_not_of(" ")) {
            // 400 Bad this->rest
        }
        this->method = this->rest.substr(0, this->rest.find(' '));
        this->rest.erase(0, this->rest.find(' '));
        this->rest.erase(0, this->rest.find_first_not_of(" "));
        this->requestTarget = this->rest.substr(0, this->rest.find(' '));
        this->rest.erase(0, this->rest.find(' '));
        this->rest.erase(0, this->rest.find_first_not_of(" "));
        this->httpVersion = this->rest.substr(0, this->rest.find(' '));
        this->rest = "";
        requestLine.erase(0, requestLine.find("\r\n") + 2);
        trackingRequestNumber++;
    }
    else
        requestLine = "";
}

// RequestLine::RequestLine(const std::string &request) {
//     // this->method = strtok((char *)requestLine.c_str(), " ");
//     // this->requestTarget = strtok(NULL, " ");
//     // this->requestTarget = strtok(NULL, " ");
//     // this->httpVersion = requestLine;
//     int index = request.find('\n');
//     std::string line = request.substr(0, index);

//     index = line.find(' ');
//     this->method = line.substr(0, index);
//     line.erase(0, index + 1);
//     index = line.find(' ');
//     this->requestTarget = line.substr(0, index);
//     line.erase(0, index + 1);
//     this->httpVersion = line;

//     // std::cout << "Method : " << this->method << std::endl;
//     // std::cout << "Request Target : " << this->requestTarget << std::endl;
//     // std::cout << "Http Version : " << this->httpVersion << std::endl;
// }

RequestLine::~RequestLine() {}
