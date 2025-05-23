/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/04/17 12:50:36 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( bool &isReadyForNextRequest ) : isReadyForNextRequest(isReadyForNextRequest) , requestLine( this->errorCode ),
                            header( this->errorCode ),
                                body( &header, this->isRequestComplete, this->errorCode ) {
    this->trackingRequestNumber     = 0;
    this->isRequestComplete         = false;
    this->checkRequestLine          = false;
    this->cgi                       = false;
    this->configFile                = NULL;
}

void Request::validateMethod(std::string &method, std::vector<std::string> &methods) {
    std::vector<std::string>::iterator it = std::find(methods.begin(), methods.end(), method);
    if (it == methods.end())
        this->errorCode = "405";
}

void Request::setCookies() {
    std::fstream outFile;
    std::string cookiesLine = this->getHeader()->getValue("COOKIE");
    while (cookiesLine.size()) {
        cookiesLine.erase(0, cookiesLine.find_first_not_of(" "));
        std::string key = cookiesLine.substr(0, cookiesLine.find("="));
        std::string value;
        cookiesLine.erase(0, cookiesLine.find("=") + 1);
        if (cookiesLine.find(";") != std::string::npos)
            value = cookiesLine.substr(0, cookiesLine.find(";"));
        else
            value = cookiesLine.substr(0);
        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == '+')
                value[i] = ' ';
        }
        cookiesLine.erase(0, value.size() + 1);
        this->cookies[key] = value;
    }
}

void Request::parseRequest ( std::string requestData ) {
    if (this->trackingRequestNumber == 0) {
        this->isReadyForNextRequest = false;
        this->requestLine.setRequestLine(requestData, this->trackingRequestNumber );
    }
    if (this->trackingRequestNumber == 1) {
        this->header.setHeader(requestData, this->trackingRequestNumber);
    }
    if (this->trackingRequestNumber == 2) {
        setCookies();
        if (!this->checkRequestLine) {
            this->configFile =  this->socket->getServerConfig(this->header.getValue("Host"));
            this->body.setConfigFile(this->socket->getServerConfig(this->header.getValue("Host")));
            this->path = this->requestLine.getRequestTarget();
            this->root = this->configFile->getRoot(this->path, this->errorCode);
            if ( this->errorCode.size() )
                return ;
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
            if (this->requestLine.getMethod() == "POST" && this->path.find(".php") != std::string::npos && this->path.find(".py") != std::string::npos) 
                this->requestTarget = this->root;
            else
                this->requestTarget = this->root + this->path;
            this->body.setRequestTarget(this->requestTarget);
            size_t      queryPos        = this->requestTarget.find("?");
            if (queryPos != std::string::npos) {
                this->queryString = this->requestTarget.substr(queryPos + 1);
                this->requestTarget.erase(queryPos);
            }
            this->body.checkAccess(this->requestTarget, this->requestLine.getMethod());
            this->index = this->configFile->getIndex();
            if ( this->requestLine.getMethod() != "DELETE" &&
                    (this->requestTarget.find(".php") != std::string::npos
                    || this->requestTarget.find(".py") != std::string::npos)) {
                this->cgiExtention = ".php";
                if (this->requestTarget.find(".py") != std::string::npos)
                    this->cgiExtention = ".py";
                std::map<std::string, std::string> mapcgi = this->location.cgi;
                if (this->location.cgi.count(this->cgiExtention)) {
                    this->cgi = true;
                }
            }
            this->checkRequestLine = true;
        }
        this->body.setBody( requestData, this->cgi, this->requestLine.getMethod() );
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
ServerConfig *Request::getConfigFile( void ) {
    return (this->configFile);
}

std::string &Request::getQueryString() {
    return (this->queryString);
}