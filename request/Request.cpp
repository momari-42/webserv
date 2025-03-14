/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/14 10:53:02 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( ) : requestLine( this->errorCode ),
                            header( this->errorCode ),
                                body( &header, this->isRequestComplete, this->errorCode ) {
    this->trackingRequestNumber     = 0;
    this->isRequestComplete         = false;
    this->checkRequestLine          = false;
    this->cgi                       = false;
}

// Red     : \033[31m
// Green   : \033[32m
// Yellow  : \033[33m
// Blue    : \033[34m
// Magenta : \033[35m
// Cyan    : \033[36m
// White   : \033[37m

void Request::validateMethod(std::string &method, std::vector<std::string> &methods) {
    std::vector<std::string>::iterator it = std::find(methods.begin(), methods.end(), method);
    if (it == methods.end())
        this->errorCode = "405";
}

static bool isDirectory(std::string &path) {
    DIR *dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return (true);
    }
    return (false);
}

void Request::parseRequest ( std::string requestData ) {
    if (this->trackingRequestNumber == 0) {
        this->requestLine.setRequestLine(requestData, this->trackingRequestNumber );
    }
    if (this->trackingRequestNumber == 1) {
        this->header.setHeader(requestData, this->trackingRequestNumber);
    }
    if (this->trackingRequestNumber == 2) {

        if (!this->checkRequestLine) {
            this->configFile =  this->socket->getServerConfig(this->header.getValue("Host"));
            this->body.setConfigFile(this->socket->getServerConfig(this->header.getValue("Host")));
            this->path = this->requestLine.getRequestTarget();
            this->root = this->configFile->getRoot(this->path, this->errorCode);
            this->location =  this->configFile->getLocations()[this->configFile->getMatchedLocation()];
            if (this->requestLine.getTempraryRequestLine().size() > this->configFile->getURILimit()) {
                this->errorCode = "414";
                return ;
            }
            validateMethod( this->requestLine.getMethod() , location.methods);
            if (this->errorCode.size())
                return ;
            if (this->root.size() && this->root.at(this->root.size() - 1) != '/')
                this->root += '/';
            if (this->path.size() && this->path.at(0) == '/')
                this->path.erase(0, 1);
            this->requestTarget = this->root + this->path;
            this->body.setRequestTarget(this->requestTarget);
            if (this->requestLine.getMethod() == "POST") {
                if (!isDirectory(this->requestTarget)) {
                    this->errorCode = "404";
                    return;
                }
            }
            this->index = this->configFile->getIndex();
            if (this->requestTarget.find(".php") != std::string::npos || this->requestTarget.find(".py") != std::string::npos) {
                this->cgiExtention = ".php";
                if (this->requestTarget.find(".py") != std::string::npos)
                    this->cgiExtention = ".py";
                if (this->location.cgi.count(this->cgiExtention))
                    this->cgi = true;
            }
            this->checkRequestLine = true;
        }
        if (this->requestLine.getMethod() == "GET" || this->requestLine.getMethod() == "DELETE") {
            this->isRequestComplete = true;
            return;
        }
        this->body.setBody( requestData, this->cgi );
    }
}

void Request::setConfigFile(ServerConfig* configFile) {
    this->configFile = configFile;
}

bool Request::getBodyComplete( void) {
    return (this->isRequestComplete);
}

size_t Request::getTrackingRequestNumber( void ) {
    return (this->trackingRequestNumber);
}


void Request::print( void ) {
    std::cout << "\033[31m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "|--------------This is the Body---------------|" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "-----------------------------------------------" << "\033[0m" << std::endl;
    this->body.printBody();
}


Request::~Request () {
}

RequestLine* Request::getRequestLine() {
    return (&this->requestLine);
}
Header* Request::getHeader() {
    return (&this->header);
}
Body* Request::getBody() {
    return (&this->body);
}

void Request::resetAttributes ( void ) {
    this->trackingRequestNumber     = 0;
    this->isRequestComplete         = false;
    this->checkRequestLine          = false;
    this->cgi                       = false;
    this->errorCode                 = "";
    this->path                      = "";
    this->root                      = "";

    this->requestLine.resetAttributes();
    this->header.resetAttributes();
    this->body.resetAttributes();
}

std::string &Request::getErrorCode() {
    return (this->errorCode);
}

std::string &Request::getRandomeFileName() {
    return(this->body.getRandomeFileName());
}

void Request::setSocket( Socket *socket ) {
    this->socket = socket;
}

std::string &Request::getPath() {
    return (this->path);
}

std::string &Request::getRoot() {
    return (this->root);
}
std::string &Request::getRequestTarget() {
    return (this->requestTarget);
}

Location &Request::getLocation() {
    return (this->location);
}

bool Request::getCgi() {
    return (this->cgi);
}

std::string Request::getCgiExtention() {
    return (this->cgiExtention);
}