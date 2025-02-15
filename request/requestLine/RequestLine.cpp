/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:12:25 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/14 21:18:05 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"

RequestLine::RequestLine( std::string &errorCode ) : errorCode(errorCode) {
    (void)this->errorCode;
}

void RequestLine::setRequestLine( std::string& requestLine, int& trackingRequestNumber ) {
    this->rest += requestLine;
    // std::cout << "a : " << this->rest << std::endl;
    if (this->rest.find("\r\n") != std::string::npos) {
        this->rest.erase(this->rest.find("\r\n"));
        if (this->rest.find('\t') != std::string::npos || this->rest.find_first_not_of(" ")) {
            this->errorCode = "400";
            return;
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
        if (this->requestTarget == "/")
            this->requestTarget = "/index.html";
        while (this->requestTarget.find("%20") != std::string::npos)
        {
            size_t pos = this->requestTarget.find("%20");
            this->requestTarget.erase(pos, 3);
            this->requestTarget.insert(pos, " ");
        }
        if ((this->method != "GET" && this->method != "POST" && this->method != "DELETE")
            || (this->requestTarget.find("/") == std::string::npos)
            ||  (this->httpVersion != "HTTP/1.1")) {
            this->errorCode = "400";
            return ;
        }
        if (this->requestTarget.size() && access( ("." + this->requestTarget).c_str(), F_OK | R_OK) == -1)  {
            this->errorCode = "404";
            return;
        }
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

RequestLine::~RequestLine() {}


void RequestLine::resetAttributes (void) {
    this->rest          = "";
    this->method        = "";
    this->requestTarget = "";
    this->httpVersion   = "";
}