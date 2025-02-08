/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/07 20:21:20 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>

Body::Body( Header *header, bool &isRequestComplete, std::string &errorCode ) : errorCode(errorCode), isRequestComplete(isRequestComplete) {
    this->header = header;
    this->bodyTrackingNumber = 0;
    this->isBodyInitiates = false;
    (void) this->errorCode;
}



void Body::printBody( void ) {
//     std::cout << this->rest.size() << std::endl;
//     std::cout << "------------------------------------------------------------------------------------" << std::endl;
    // std::cout << this->body  << std::endl;
}

Body::~Body() {}

void Body::parseBoundaryHeader(const std::string& header) {
    boundaryData_t part;
    part.isFile = false;
    part.isHeaderComplete = false;
    part.isBodyComplete = false;
    part.name = header.substr(header.find("name=\"") + 6);
    part.name = part.name.substr(0, part.name.find("\""));
    if (header.find("filename=\"") != std::string::npos) {
        part.contenet = header.substr(header.find("filename=\"") + 10);
        part.contenet = part.contenet.substr(0, part.contenet.find("\""));
        part.isFile = true;
    }
    this->data.push_back(part);
}

void manageFile(const std::string fileName, const std::string data ) {
    std::ofstream outputFile( "/Users/momari/goinfre/upload/" + fileName, std::ios::binary | std::ios::app);
    outputFile.write(data.data(), data.size());
}

void Body::setBoundaryBody( std::string& requestData, const std::string& token ) {
    this->rest += requestData;

    while (this->rest.size() && !this->isRequestComplete) {
        if (this->rest.find(token + "--") != std::string::npos && this->rest.find(token) == this->rest.find(token + "--")) {
            if (!this->data.back().isBodyComplete)
                manageFile(this->data.back().contenet, this->rest.substr(0, this->rest.find(token + "--") - 2));
            this->isRequestComplete = true;
            this->rest = "";
            break;
        }
        if (this->rest.find(token) != std::string::npos && this->rest.find("\r\n\r\n") != std::string::npos) {
            if (this->rest.find(token) != 0) {
                manageFile(this->data.back().contenet, this->rest.substr(0, this->rest.find(token) - 2));
                this->rest.erase(0, this->rest.find(token));
                this->data.back().isBodyComplete = true;
            }
            if (this->rest.find(token) == 0 && this->rest.find("\r\n\r\n") != std::string::npos) {
                this->rest.erase(0, token.size() + 2);
                Body::parseBoundaryHeader(this->rest.substr(0, this->rest.find("\r\n\r\n")));
                this->data.back().isHeaderComplete = true;
                this->rest.erase(0, this->rest.find("\r\n\r\n") + 4);
            }
        }
        else if (this->data.size() && this->data.back().isHeaderComplete && !this->data.back().isBodyComplete) {
            if ( (this->rest.find_last_of(token) == std::string::npos) || ( this->rest.size() - this->rest.find_last_of(token) > token.size() )) {
                manageFile(this->data.back().contenet, this->rest); 
                this->rest = "";
                break;
            }
            else break;
        }
        else
        
            break;
    }
    requestData = "";
}

void Body::setBoundaryChunkedBody( std::string& body) {
    Body::setChunkedBody(body);
    Body::setBoundaryBody(this->body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
}

void Body::initiateBodyParams( void ) {
    if (this->header->getValue("Content-Type") != "" && this->header->getValue("Content-Type").find("boundary") != std::string::npos
        && this->header->getValue("Transfer-Encoding") == "chunked")
        this->bodyRequestType = "chunkedboundry";
    else if (this->header->getValue("Content-Type") != "" && this->header->getValue("Content-Type").find("boundary") != std::string::npos)
        this->bodyRequestType = "boundry";
    else if (this->header->getValue("Transfer-Encoding") == "chunked")
        this->bodyRequestType = "chunked";
    else if (this->header->getValue("Content-Length") != "") {
        this->contentLength = strtol(this->header->getValue("Content-Length").c_str(), NULL, 0);
        this->bodyRequestType = "contentlength";
    }
}

void Body::setBody( std::string& body ) {
    if (!this->isBodyInitiates) {
        initiateBodyParams();
        this->isBodyInitiates = true;
    }
    if (this->bodyRequestType == "chunked")
        Body::setChunkedBody(body);
    else if (this->bodyRequestType == "boundry")
        Body::setBoundaryBody(body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
    else if (this->bodyRequestType == "chunkedboundry")
        Body::setBoundaryChunkedBody( body );
}

void Body::setChunkedBody( std::string& body ) {
    this->restChunked += body;
    while ( this->restChunked.size() )
    {
        if ( !this->bodyTrackingNumber  && this->restChunked.find("\r\n") == std::string::npos )
            break;
        if ( !this->bodyTrackingNumber  && this->restChunked.find("\r\n") != std::string::npos ) {
            this->bodyTrackingNumber = strtol(this->restChunked.substr(0, this->restChunked.find("\r\n")).c_str(), NULL, 16);
            this->restChunked.erase(0, this->restChunked.find("\r\n") + 2);
            if (this->bodyTrackingNumber == 0) {
                this->isRequestComplete = true;
                this->restChunked = "";
                break;
            }
        }
        if ( this->bodyTrackingNumber && this->restChunked.size() <= this->bodyTrackingNumber ) {
            this->body += this->restChunked;
            this->bodyTrackingNumber -= this->restChunked.size();
            this->restChunked = "";
        }
        else if ( this->bodyTrackingNumber ) {
            this->body += this->restChunked.substr(0, this->bodyTrackingNumber);
            this->restChunked.erase(0, this->bodyTrackingNumber + 2);
            this->bodyTrackingNumber = 0;
        }
    }
    body = "";
}
