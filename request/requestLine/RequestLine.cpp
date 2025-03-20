/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:12:25 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/20 15:36:48 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"

RequestLine::RequestLine( std::string &errorCode ) : errorCode(errorCode) {
    (void)this->errorCode;
} 

void RequestLine::validateMethod( ) {
    // std::cout << "the method is : " << this->method << std::endl;
    if ((this->method != "GET" && this->method != "POST" && this->method != "DELETE")
        || (this->requestTarget.find("/") == std::string::npos)
        ||  (this->httpVersion != "HTTP/1.1")) {
        // std::cout <<  "from validateMethod" << std::endl;
        this->errorCode = "400";
        return ;
    }
}


static char hexToChar(const std::string hexString) {
    int result = 0;
    std::istringstream iss(hexString);
    iss >> std::hex >> result;
    return static_cast<char>(result);
}

static bool isHexadecimal( std::string &hexString ) {
    for (size_t n = 1; n < hexString.size(); n++ ){
        if ( !std::isxdigit(hexString.at(n)) )
            return (false);
    }
    return (true);
}

static void  decodeUrlEncodedCharacters( std::string &requestTarget ) {
    std::string portion;
    char        sCharacter;

    while ( requestTarget.find("%") != std::string::npos ) {
        portion = requestTarget.substr(requestTarget.find("%"));
        size_t pos = requestTarget.find("%");    
        if (portion.size() < 3)
            break;
        portion = portion.substr(0, 3);
        if ( isHexadecimal( portion) ) {
            sCharacter = hexToChar(portion.substr(1, 2));
            requestTarget.erase(pos, 3);
            requestTarget.insert(pos, 1, sCharacter);
        } else {
            requestTarget.erase(pos, 1);
            requestTarget.insert(pos, 1, '\x15');
        }
    }
    while ( requestTarget.find('\x15') != std::string::npos ) {
        size_t pos = requestTarget.find('\x15');
        requestTarget.erase(pos, 1);
        requestTarget.insert(pos, 1, '%');
    }
}

void RequestLine::setRequestLine( std::string& requestLine, int& trackingRequestNumber ) {
    this->rest += requestLine;
    if (this->rest.find("\r\n") != std::string::npos) {
        this->tempraryRequestLine = this->rest.substr(0, this->rest.find("\r\n"));
        this->rest.erase(this->rest.find("\r\n"));
        if (this->rest.find('\t') != std::string::npos || this->rest.find_first_not_of(" ")) {
            // std::cout << "from set request line" << std::endl;
            this->errorCode = "400";
            return;
        }
        std::vector<std::string> parts;
        ft_split(this->tempraryRequestLine, parts);
        if (parts.size() != 3) {
            this->errorCode = "400";
            return;
        }
        this->method = parts[0];
        this->requestTarget = parts[1];
        this->httpVersion = parts[2];
        if (this->requestTarget.find("?") != std::string::npos) {
            this->data = this->requestTarget.substr(this->requestTarget.find("?") + 1);
            this->path = this->requestTarget.substr(0, this->requestTarget.find("?"));
        }
        this->rest = "";
        this->tempraryRequestLine = "";
        requestLine.erase(0, requestLine.find("\r\n") + 2);
        // std::cout << "  This is the request target before the changes :" << this->requestTarget << std::endl;
        decodeUrlEncodedCharacters(this->requestTarget);
        // std::cout << "  This is the request target after  the changes :" << this->requestTarget << std::endl;
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

std::string &RequestLine::getPath ( void ) {
    return (this->path);
}

std::string &RequestLine::getData ( void ) {
    return (this->data);
}

std::string &RequestLine::getTempraryRequestLine ( void ) {
    return (this->tempraryRequestLine);
}

RequestLine::~RequestLine() {}


void RequestLine::resetAttributes (void) {
    this->rest                  = "";
    this->method                = "";
    this->requestTarget         = "";
    this->httpVersion           = "";
    this->tempraryRequestLine   = "";
}