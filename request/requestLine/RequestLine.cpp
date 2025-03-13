/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:12:25 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/13 14:39:27 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"

RequestLine::RequestLine( std::string &errorCode ) : errorCode(errorCode) {
    (void)this->errorCode;
} 

void RequestLine::validateMethod( ) {
    std::cout << "the method is : " << this->method << std::endl;
    if ((this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        || (this->requestTarget.find("/") == std::string::npos)
        ||  (this->httpVersion != "HTTP/1.1")) {
        std::cout <<  "from validateMethod" << std::endl;
        this->errorCode = "400";
        return ;
    }
}


void RequestLine::setRequestLine( std::string& requestLine, int& trackingRequestNumber ) {
    this->rest += requestLine;
    if (this->rest.find("\r\n") != std::string::npos) {
        this->tempraryRequestLine = this->rest.substr(0, this->rest.find("\r\n"));
        this->rest.erase(this->rest.find("\r\n"));
        if (this->rest.find('\t') != std::string::npos || this->rest.find_first_not_of(" ")) {
            std::cout <<  "from setRequestLine" << std::endl;
            this->errorCode = "400";
            return;
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
        while (this->requestTarget.find("%20") != std::string::npos)
        {
            size_t pos = this->requestTarget.find("%20");
            this->requestTarget.erase(pos, 3);
            this->requestTarget.insert(pos, " ");
        }
        validateMethod();
        if (this->errorCode.size())
            return ;
        trackingRequestNumber++;
    }
    else
        requestLine = "";
}

std::string &RequestLine::getMethod ( void ) {
    return (this->method);
}

std::string &RequestLine::getRequestTarget ( void ) {
    return (this->requestTarget);
}

std::string &RequestLine::getTempraryRequestLine ( void ) {
    return (this->tempraryRequestLine);
}

RequestLine::~RequestLine() {}


void RequestLine::resetAttributes (void) {
    this->rest          = "";
    this->method        = "";
    this->requestTarget = "";
    this->httpVersion   = "";
}