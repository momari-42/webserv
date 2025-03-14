/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/13 22:55:03 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>


Body::Body( Header *header, bool &isRequestComplete, std::string &errorCode ) : errorCode(errorCode), isRequestComplete(isRequestComplete){
    this->header                = header;
    this->bodyTrackingNumber    = 0;
    this->isBodyInitiates       = false;
    this->isShunked             = false;
    this->bodyLength            = 0;
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

void Body::manageFile(const std::string fileName, const std::string data ) {
    std::ofstream outputFile( this->requestTarget + fileName, std::ios::binary | std::ios::app);
    outputFile.write(data.data(), data.size());
}

void Body::validateFileName( void ) {
    if ( this->data.back().contenet.empty() ) {
        for ( std::vector<boundaryData_t>::iterator it = this->data.begin(); it != this->data.end() - 1; it++ ) {
            unlink(( this->requestTarget + it->contenet).c_str());
        }
        std::cout << "from validateFileName" << std::endl;
        this->errorCode = "400";
    }
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
                // this is for validate each file it has file name or not 
                validateFileName();
                if ( this->errorCode.size() )
                    return ;

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

void Body::setCgi( std::string& requestData ) {
    if (this->randomeFileName.empty()) {
        this->isShunked = this->bodyRequestType.find("chunked") != std::string::npos;
        generateRandomeName( this->randomeFileName );
        this->randomeFileName = "/tmp/." + this->randomeFileName;
    }
    if ( this->isShunked )
        setChunkedCgiBody( requestData );
    else
        manageFile(this->randomeFileName, requestData);
}

void Body::setBody( std::string& body, bool &cgi ) {
    if (!this->isBodyInitiates) {
        initiateBodyParams();
        this->cgi = cgi;
        this->isBodyInitiates = true;
    }
    this->bodyLength += body.size();
    if (this->bodyLength > this->configFile->getBodyLimit()) {
        this->errorCode = "413";
        return;
    }
    if ( cgi )
        Body::setCgi(body);
    if (this->bodyRequestType == "chunked")
        Body::setChunkedBody(body);
    else if (this->bodyRequestType == "boundry")
        Body::setBoundaryBody(body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
    else if (this->bodyRequestType == "chunkedboundry")
        Body::setBoundaryChunkedBody( body );
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
    this->isShunked                 = false;

    this->randomeFileName           = "";
    this->randomeContentLengthName  = "";
    this->restChunked               = "";
    this->body                      = "";
    this->rest                      = "";
    this->bodyRequestType           = "";
    this->bodyLength                = 0;
    this->contentLength             = 0;
    this->data                      = tmp;
}



void Body::setContentLengthBody( std::string& requestData ) {
    if ( !this->randomeContentLengthName.size() ) {
        generateRandomeName ( this->randomeContentLengthName );
    }
    if ( this->contentLength - requestData.size() < 0 )
        manageFile(this->randomeFileName, requestData.substr(0, this->contentLength));
    else
        manageFile(this->randomeFileName, requestData);
    this->contentLength -= requestData.size();
    if ( this->contentLength <= 0 ) {
        this->isRequestComplete = true;
    }
}

void Body::setChunkedCgiBody( std::string& body ) {
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
                this->isRequestComplete = true;
                this->restChunked = "";
                break;
            }
        }
        if ( this->bodyTrackingNumber && this->restChunked.size() <= this->bodyTrackingNumber ) {
            manageFile(this->randomeFileName, this->restChunked);
            this->bodyTrackingNumber -= this->restChunked.size();
            this->restChunked.clear();
        }
        else if ( this->bodyTrackingNumber ) {
            manageFile(this->randomeFileName, this->restChunked.substr(0, this->bodyTrackingNumber));
            this->restChunked.erase(0, this->bodyTrackingNumber + 2);
            this->bodyTrackingNumber = 0;
        }
    }
    body = "";
}

void Body::setChunkedBody( std::string& body ) {

    if ( this->bodyRequestType == "chunked" && !this->cgi ) {
        this->errorCode = "400";
        return ;
    }
    if (this->bodyRequestType == "chunked" && !this->randomeFileName.size())
        generateRandomeName( this->randomeFileName );
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
                manageFile(this->randomeFileName, this->restChunked);
            else {
                this->body += this->restChunked;
            }
            this->bodyTrackingNumber -= this->restChunked.size();
            this->restChunked.clear();
        }
        else if ( this->bodyTrackingNumber ) {
            if (this->bodyRequestType == "chunked")
                manageFile(this->randomeFileName, this->restChunked.substr(0, this->bodyTrackingNumber));
            else
                this->body += this->restChunked.substr(0, this->bodyTrackingNumber);
            this->restChunked.erase(0, this->bodyTrackingNumber + 2);
            this->bodyTrackingNumber = 0;
        }
    }
    body = "";
}

std::string &Body::getRandomeFileName() {
    return (this->randomeFileName);
}

void Body::setConfigFile(ServerConfig* configFile) {
    this->configFile = configFile;
}

static bool isDirectory(std::string &path) {
    DIR *dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return (true);
    }
    return (false);
}

void Body::checkAccess( std::string &requestTarget ) {
    if (isDirectory(requestTarget)) {
        bool isValidPath = false;
        std::vector<std::string> &index = this->configFile->getIndex();
        std::string tempraryRequestTarget;

        if (requestTarget.size() && requestTarget.at(requestTarget.size() - 1) != '/')
            requestTarget += '/';
        for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
            tempraryRequestTarget = requestTarget + *it;
            if (access( tempraryRequestTarget.c_str(), F_OK ) != -1) {
                requestTarget = tempraryRequestTarget;
                isValidPath = true;
                break;
            }
        }
        if (!isValidPath) {
            this->errorCode = "404";
            return ;
        }
    }
    else {
        if (access( requestTarget.c_str(), F_OK ) == -1) {
            this->errorCode = "404";
            return;
        }   
        if (access( requestTarget.c_str(), R_OK ) == -1) {
            this->errorCode = "401";
            return;
        }
    }
}

void Body::setRequestTarget(std::string &requestTarget) {
    this->requestTarget = requestTarget;
}

std::string Body::getBodyRequestType() {
    return ( this->bodyRequestType );
}
size_t Body::getBodyLength() {
    return ( this->bodyLength );
}