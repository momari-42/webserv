/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/06 13:33:23 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>


Body::Body( Header *header, bool &isRequestComplete, std::string &errorCode ) : errorCode(errorCode), isRequestComplete(isRequestComplete){
    this->header                = header;
    this->bodyTrackingNumber    = 0;
    this->isBodyInitiates       = false;
    this->bodyLength            = 0;
    // setMime();
    (void) this->errorCode;
}



void Body::printBody( void ) {
//     std::cout << this->rest.size() << std::endl;
//     std::cout << "------------------------------------------------------------------------------------" << std::endl;
    // std::cout << this->body  << std::endl;
}

Body::~Body() {}

void parseFileName(std::string &fileName) {
    std::string extPortion;
    if (fileName.find_last_of(".") != std::string::npos) {
        extPortion = fileName.substr(fileName.find_last_of("."));
        fileName.erase(fileName.find_last_of("."));
    }
    while (access(("upload/" + fileName + extPortion).c_str(), F_OK) == 0) {
        fileName += "_";
        // std::cout << "inf loop" << std::endl;
    }
    fileName += extPortion;
}

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
        parseFileName(part.contenet);
        part.isFile = true;
    }
    this->data.push_back(part);
}

void manageFile(const std::string fileName, const std::string data ) {
    // std::cout << fileName << std::endl;
    std::ofstream outputFile( "/Users/momari/goinfre/" + fileName, std::ios::binary | std::ios::app);
    outputFile.write(data.data(), data.size());
}

void Body::setBoundaryBody( std::string& requestData, const std::string& token ) {
    this->rest += requestData;

    while (this->rest.size() && !this->isRequestComplete) {
        // std::cout <<  "momari" << std::endl;
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
        this->bodyRequestType = "Content-Length";
    }
}

void Body::setBody( std::string& body ) {
    if (!this->isBodyInitiates) {
        initiateBodyParams();
        this->isBodyInitiates = true;
    }
    this->bodyLength += body.size();
    if (this->bodyLength > this->configFile->getBodyLimit()) {
        this->errorCode = "413";
        return;
    }
    if (this->bodyRequestType == "chunked") {
        Body::setChunkedBody(body);
    }
    else if (this->bodyRequestType == "boundry") {
        Body::setBoundaryBody(body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
    }
    else if (this->bodyRequestType == "chunkedboundry") {
        Body::setBoundaryChunkedBody( body );
    }
    else if (this->bodyRequestType == "Content-Length") {
        if ( this->contentLength > this->configFile->getBodyLimit() ) {
            this->errorCode = "413";
            return;
        }
        Body::setContentLengthBody( body );
    }
}

void Body::generateRandomeName( std::string& name ) {
    std::string buffer = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    size_t      sizeName = 12;
    size_t      randomNumber;

    srand(time(0));

    while (name.size() != sizeName) {
        randomNumber = rand() % buffer.size();
        name += buffer.at(randomNumber);
    }
    if ( this->header->getValue("Content-Type") != "" ) {
        std::map<std::string, std::string>::iterator it = this->emim.find(this->header->getValue("Content-Type"));
        if ( it != this->emim.end()) {
            name += it->second;
        }
    }
}


void Body::resetAttributes (void) {
    std::vector<boundaryData_t>     tmp;
    this->bodyTrackingNumber        = 0;
    this->isBodyInitiates           = false;

    this->randomeChunkedName        = "";
    this->randomeContentLengthName  = "";
    this->restChunked               = "";
    this->body                      = "";
    this->rest                      = "";
    this->bodyRequestType           = "";
    this->data                      = tmp;
    this->contentLength             = 0;
    this->bodyLength                = 0;
}



void Body::setContentLengthBody( std::string& requestData ) {
    if ( !this->randomeContentLengthName.size() ) {
        generateRandomeName ( this->randomeContentLengthName );
    }
    if ( this->contentLength - requestData.size() < 0 )
        manageFile(this->randomeChunkedName, requestData.substr(0, this->contentLength));
    else
        manageFile(this->randomeChunkedName, requestData);
    this->contentLength -= requestData.size();
    if ( this->contentLength <= 0 ) {
        this->isRequestComplete = true;
    }
}

void Body::setChunkedBody( std::string& body ) {

    if (this->bodyRequestType == "chunked" && !this->randomeChunkedName.size())
        generateRandomeName( this->randomeChunkedName );
    this->restChunked += body;
    while ( this->restChunked.size() )
    {
        if ( !this->bodyTrackingNumber  && this->restChunked.find("\r\n") == std::string::npos ) {
            break;
        }
        if ( !this->bodyTrackingNumber  && this->restChunked.find("\r\n") != std::string::npos ) {
            this->bodyTrackingNumber = strtol(this->restChunked.substr(0, this->restChunked.find("\r\n")).c_str(), NULL, 16);
            this->restChunked.erase(0, this->restChunked.find("\r\n") + 2);
            if (this->bodyTrackingNumber == 0) {
                if (this->bodyRequestType == "chunked")
                    this->isRequestComplete = true;
                this->restChunked = "";
                break;
            }
        }
        if ( this->bodyTrackingNumber && this->restChunked.size() <= this->bodyTrackingNumber ) {
            if (this->bodyRequestType == "chunked")
                manageFile(this->randomeChunkedName, this->restChunked);
            else {
                this->body += this->restChunked;
            }
            this->bodyTrackingNumber -= this->restChunked.size();
            this->restChunked.clear();
        }
        else if ( this->bodyTrackingNumber ) {
            if (this->bodyRequestType == "chunked")
                manageFile(this->randomeChunkedName, this->restChunked.substr(0, this->bodyTrackingNumber));
            else {
                // std::cout << "woooooooow " << std::endl;
                this->body += this->restChunked.substr(0, this->bodyTrackingNumber);
            }
            this->restChunked.erase(0, this->bodyTrackingNumber + 2);
            this->bodyTrackingNumber = 0;
        }
    }
    // std::cout << this->body << std::endl;
    body = "";
}

std::string &Body::getFileName() {
    return (this->randomeContentLengthName);
}

void Body::setConfigFile(ServerConfig* configFile) {
    this->configFile = configFile;
}