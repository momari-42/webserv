/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/20 02:49:27 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>


Body::Body( Header *header, bool &isRequestComplete, std::string &errorCode ) : errorCode(errorCode), isRequestComplete(isRequestComplete){
    this->header                = header;
    this->bodyTrackingNumber    = 0;
    this->isBodyInitiates       = false;
    this->isShunked             = false;
    this->created               = false;
    this->contentLength         = -1;
    this->bodyLength            = 0;
    (void) this->errorCode;
}



void Body::printBody( void ) {
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
        part.content = header.substr(header.find("filename=\"") + 10);
        part.content = part.content.substr(0, part.content.find("\""));
        // parseFileName(part.content);
        part.isFile = true;
    }
    this->data.push_back(part);
}

void Body::manageExistinceFile(std::string &fileName) {
    if (this->method == "POST") {
        if ( !access((this->requestTarget + fileName).c_str(), F_OK) ) {
            if (!access((this->requestTarget + fileName).c_str(), R_OK))
                unlink((this->requestTarget + fileName).c_str());
            else
                parseFileName(fileName);
        }
    }
}

void Body::manageFile(const std::string fileName, const std::string data ) {
    if (this->method == "POST") {
        std::string path;
        if (this->cgi)
            path = fileName;
        else
            path = this->requestTarget + fileName;
        // std::cout << "this is the request target : " << this->requestTarget << std::endl;
        std::ofstream outputFile( path, std::ios::binary | std::ios::app);
        outputFile.write(data.data(), data.size());
        this->created               = true;
    }
}

void Body::validateFileName( void ) {
    if ( this->data.back().content.empty() ) {
        for ( std::vector<boundaryData_t>::iterator it = this->data.begin(); it != this->data.end() - 1; it++ ) {
            unlink(( this->requestTarget + it->content).c_str());
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
                manageFile(this->data.back().content, this->rest.substr(0, this->rest.find(token + "--") - 2));
            this->isRequestComplete = true;
            this->rest.erase(0, this->rest.find(token + "--") + token.size() + 4);
            this->nextRequest = this->rest;
            this->rest = "";
            break;
        }
        if (this->rest.find(token) != std::string::npos && this->rest.find("\r\n\r\n") != std::string::npos) {
            if (this->rest.find(token) != 0) {
                manageFile(this->data.back().content, this->rest.substr(0, this->rest.find(token) - 2));
                this->rest.erase(0, this->rest.find(token));
                this->data.back().isBodyComplete = true;
            }
            if (this->rest.find(token) == 0 && this->rest.find("\r\n\r\n") != std::string::npos) {
                this->rest.erase(0, token.size() + 2);
                Body::parseBoundaryHeader(this->rest.substr(0, this->rest.find("\r\n\r\n")));
                if (this->method == "POST") {
                    validateFileName();
                    if ( this->errorCode.size() )
                        return ;
                    manageExistinceFile(this->data.back().content);
                }
                this->data.back().isHeaderComplete = true;
                this->rest.erase(0, this->rest.find("\r\n\r\n") + 4);
            }
        }
        else if (this->data.size() && this->data.back().isHeaderComplete && !this->data.back().isBodyComplete) {
            if ( (this->rest.find_last_of(token) == std::string::npos) || ( this->rest.size() - this->rest.find_last_of(token) > token.size() )) {
                manageFile(this->data.back().content, this->rest); 
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
    // std::cout<< "from here here here here here here here " << std::endl;
    Body::setChunkedBody(body);
    Body::setBoundaryBody(this->body, "--" + this->header->getValue("CONTENT_TYPE").substr(this->header->getValue("CONTENT_TYPE").find("boundary=") + 9));
}

void Body::initiateBodyParams( void ) {
    if (this->header->getValue("CONTENT_TYPE") != "" && this->header->getValue("CONTENT_TYPE").find("boundary") != std::string::npos
        && this->header->getValue("TRANSFER_ENCODING") == "chunked")
        this->bodyRequestType = "chunkedboundry";
    else if (this->header->getValue("CONTENT_TYPE") != "" && this->header->getValue("CONTENT_TYPE").find("boundary") != std::string::npos)
        this->bodyRequestType = "boundry";
    else if (this->header->getValue("TRANSFER_ENCODING") == "chunked")
        this->bodyRequestType = "chunked";
    else if (this->header->getValue("CONTENT_LENGTH") != "") {
        this->bodyRequestType = "Content-Length";
    }
    if (!this->header->getValue("CONTENT_LENGTH").empty())
        this->contentLength = strtol(this->header->getValue("CONTENT_LENGTH").c_str(), NULL, 0);
}

void Body::setCgi( std::string& requestData ) {
    if (this->randomeFileName.empty()) {
        this->isShunked = this->bodyRequestType.find("chunked") != std::string::npos;
        generateRandomeName( this->randomeFileName );
        this->randomeFileName = "/tmp/." + this->randomeFileName;
        if ( this->bodyRequestType == "boundry" )
            this->boundryToken = "--" + this->header->getValue("CONTENT_TYPE").substr(this->header->getValue("CONTENT_TYPE").find("boundary=") + 9);
    }
    if ( this->isShunked )
        setChunkedCgiBody( requestData );
    else if (this->bodyRequestType == "boundry" || this->bodyRequestType == "Content-Length")
    {
        if ( this->bodyRequestType == "boundry" && requestData.find(this->boundryToken + "--") != std::string::npos )
            this->isRequestComplete = true;
        else if (this->bodyRequestType == "Content-Length" ) {
            this->contentLength -= requestData.size();
            if (this->contentLength <= 0)
                this->isRequestComplete = true;
        }
        manageFile(this->randomeFileName, requestData);
    }
    else
        this->isRequestComplete = true;
}

void Body::unlinkCreatedFiles( std::vector<boundaryData_t> &data ) {
    for (std::vector<boundaryData_t>::iterator it = data.begin(); it < data.end(); it++) {
        unlink ((this->requestTarget + it->content).c_str());
    }
}

static bool isDirectory(std::string &path) {
    DIR *dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return (true);
    }
    return (false);
}

void Body::setBody( std::string& body, bool &cgi, std::string &method ) {
    if (!this->isBodyInitiates) {
        initiateBodyParams();
        this->cgi = cgi;
        this->method = method;
        this->isBodyInitiates = true;
        if (this->contentLength > static_cast<ssize_t>(this->configFile->getBodyLimit())) {
            this->errorCode = "413";
            return;
        }
        if ( !this->cgi && this->method == "POST" && !isDirectory(this->requestTarget) ) {
            std::cout << "from set body" << this->cgi << std::endl;
            this->errorCode = "400";
            return;
        }
    }
    this->bodyLength += body.size();
    if ( (this->method == "GET" || this->method == "DELETE") && this->bodyLength > 1024 ) {
        this->errorCode = "413";
        return;
    }
    if (this->contentLength == -1 && (this->bodyLength > static_cast<ssize_t>(this->configFile->getBodyLimit()))) {
        if ( this->data.size() )
            unlinkCreatedFiles(data);
        this->errorCode = "413";
        return;
    }
    if ( this->cgi )
        Body::setCgi(body);
    else if (this->bodyRequestType == "chunked")
        Body::setChunkedBody(body);
    else if (this->bodyRequestType == "boundry")
        Body::setBoundaryBody(body, "--" + this->header->getValue("CONTENT_TYPE").substr(this->header->getValue("CONTENT_TYPE").find("boundary=") + 9));
    else if (this->bodyRequestType == "chunkedboundry")
        Body::setBoundaryChunkedBody( body );
    else if (this->bodyRequestType == "Content-Length")
        Body::setContentLengthBody( body );
    else {
        nextRequest = body;
        this->isRequestComplete = true;
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
    if ( !this->header->getValue("CONTENT_TYPE").empty() ) {
        std::map<std::string, std::string>::iterator it = this->emim.find(this->header->getValue("CONTENT_TYPE"));
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
    this->created                   = false;

    this->randomeFileName           = "";
    this->randomeContentLengthName  = "";
    this->restChunked               = "";
    this->body                      = "";
    this->rest                      = "";
    this->bodyRequestType           = "";
    this->bodyLength                = 0;
    this->contentLength             = -1;
    this->data                      = tmp;
}



void Body::setContentLengthBody( std::string& requestData ) {
    if ( this->bodyRequestType == "Content-Length" && !this->cgi ) {
        // std::cout << "from chunked" << std::endl;
        if ( !this->contentLength ) {
            this->isRequestComplete = true;
            return;
        }
        this->errorCode = "400";
        return ;
    }
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
        std::cout << "from chunked" << std::endl;
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
            // std::cout << "from check access" << std::endl;
            this->errorCode = "404";
            return ;
        }
    }
    else {
        if (access( requestTarget.c_str(), F_OK ) == -1) {
            // std::cout << "from check access 11" << std::endl;
            this->errorCode = "404";
            return;
        }   
        if (access( requestTarget.c_str(), R_OK ) == -1) {
            // std::cout << "from check access 2" << std::endl;
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
// size_t Body::getBodyLength() {
//     return ( this->bodyLength );
// }

std::string &Body::getNextRequest() {
    return (this->nextRequest);
}
void Body::setNextRequest( std::string nextRequest ) {
    this->nextRequest = nextRequest;
}

bool Body::getCreated() {
    return (this->created);
}