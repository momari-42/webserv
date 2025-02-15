/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/13 17:38:11 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>

void Body::setMime() {
    this->mime["audio/aac"]                                                                 = ".aac"    ;
    this->mime["application/x-abiword"]                                                     = ".abw"    ;
    this->mime["image/apng"]                                                                = ".apng"   ;
    this->mime["application/x-freearc"]                                                     = ".arc"    ;
    this->mime["image/avif"]                                                                = ".avif"   ;
    this->mime["video/x-msvideo"]                                                           = ".avi"    ;
    this->mime["application/vnd.amazon.ebook"]                                              = ".azw"    ;
    this->mime["application/octet-stream"]                                                  = ".bin"    ;
    this->mime["image/bmp"]                                                                 = ".bmp"    ;
    this->mime["application/x-bzip"]                                                        = ".bz"     ;
    this->mime["application/x-bzip2"]                                                       = ".bz2"    ;
    this->mime["application/x-cdf"]                                                         = ".cda"    ;
    this->mime["application/x-csh"]                                                         = ".csh"    ;
    this->mime["text/css"]                                                                  = ".css"    ;
    this->mime["text/csv"]                                                                  = ".csv"    ;
    this->mime["application/msword"]                                                        = ".doc"    ;
    this->mime["application/vnd.openxmlformats-officedocument.wordprocessingml.document"]   = ".docx"   ;
    this->mime["application/vnd.ms-fontobject"]                                             = ".eot"    ;
    this->mime["application/epub+zip"]                                                      = ".epub"   ;
    this->mime["application/gzip"]                                                          = ".gz"     ;
    this->mime["image/gif"]                                                                 = ".gif"    ;
    this->mime["text/html"]                                                                 = ".htm"    ;
    this->mime["text/html"]                                                                 = ".html"   ;
    this->mime["image/vnd.microsoft.icon"]                                                  = ".ico"    ;
    this->mime["text/calendar"]                                                             = ".ics"    ;
    this->mime["application/java-archive"]                                                  = ".jar"    ;
    this->mime["image/jpeg"]                                                                = ".jpeg"   ;
    this->mime["image/jpeg"]                                                                = ".jpg"    ;
    this->mime["text/javascript"]                                                           = ".js"     ;
    this->mime["application/json"]                                                          = ".json"   ;
    this->mime["application/ld+json"]                                                       = ".jsonld" ;
    this->mime["audio/midi"]                                                                = ".mid"    ;
    this->mime["audio/midi"]                                                                = ".midi"   ;
    this->mime["text/javascript"]                                                           = ".mjs"    ;
    this->mime["audio/mpeg"]                                                                = ".mp3"    ;
    this->mime["video/mp4"]                                                                 = ".mp4"    ;
    this->mime["video/mpeg"]                                                                = ".mpeg"   ;
    this->mime["application/vnd.apple.installer+xml"]                                       = ".mpkg"   ;
    this->mime["application/vnd.oasis.opendocument.presentation"]                           = ".odp"    ;
    this->mime["application/vnd.oasis.opendocument.spreadsheet"]                            = ".ods"    ;
    this->mime["application/vnd.oasis.opendocument.text"]                                   = ".odt"    ;
    this->mime["audio/ogg"]                                                                 = ".oga"    ;
    this->mime["video/ogg"]                                                                 = ".ogv"    ;
    this->mime["application/ogg"]                                                           = ".ogx"    ;
    this->mime["audio/ogg"]                                                                 = ".opus"   ;
    this->mime["font/otf"]                                                                  = ".otf"    ;
    this->mime["image/png"]                                                                 = ".png"    ;
    this->mime["application/pdf"]                                                           = ".pdf"    ;
    this->mime["application/x-httpd-php"]                                                   = ".php"    ;
    this->mime["application/vnd.ms-powerpoint"]                                             = ".ppt"    ;
    this->mime["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = ".pptx"   ;
    this->mime["application/vnd.rar"]                                                       = ".rar"    ;
    this->mime["application/rtf"]                                                           = ".rtf"    ;
    this->mime["application/x-sh"]                                                          = ".sh"     ;
    this->mime["image/svg+xml"]                                                             = ".svg"    ;
    this->mime["application/x-tar"]                                                         = ".tar"    ;
    this->mime["image/tiff"]                                                                = ".tif"    ;
    this->mime["image/tiff"]                                                                = ".tiff"   ;
    this->mime["video/mp2t"]                                                                = ".ts"     ;
    this->mime["font/ttf"]                                                                  = ".ttf"    ;
    this->mime["text/plain"]                                                                = ".txt"    ;
    this->mime["application/vnd.visio"]                                                     = ".vsd"    ;
    this->mime["audio/wav"]                                                                 = ".wav"    ;
    this->mime["audio/webm"]                                                                = ".weba"   ;
    this->mime["video/webm"]                                                                = ".webm"   ;
    this->mime["image/webp"]                                                                = ".webp"   ;
    this->mime["font/woff"]                                                                 = ".woff"   ;
    this->mime["font/woff2"]                                                                = ".woff2"  ;
    this->mime["application/xhtml+xml"]                                                     = ".xhtml"  ;
    this->mime["application/vnd.ms-excel"]                                                  = ".xls"    ;
    this->mime["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"]         = ".xlsx"   ;
    this->mime["application/xml"]                                                           = ".xml"    ;
    this->mime["application/vnd.mozilla.xul+xml"]                                           = ".xul"    ;
    this->mime["application/zip"]                                                           = ".zip"    ;
    this->mime["video/3gpp"]                                                                = ".3gp"    ;
    this->mime["video/3gpp2"]                                                               = ".3g2"    ;
    this->mime["application/x-7z-compressed"]                                               = ".7z"     ;
}


Body::Body( Header *header, bool &isRequestComplete, std::string &errorCode ) : errorCode(errorCode), isRequestComplete(isRequestComplete) {
    this->header = header;
    this->bodyTrackingNumber = 0;
    this->isBodyInitiates = false;
    setMime();
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
    std::ofstream outputFile( "upload/" + fileName, std::ios::binary | std::ios::app);
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
    if (this->bodyRequestType == "chunked") {
        // std::cout << "1" << std::endl;
        Body::setChunkedBody(body);
    }
    else if (this->bodyRequestType == "boundry") {
        // std::cout << "2" << std::endl;
        Body::setBoundaryBody(body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
    }
    else if (this->bodyRequestType == "chunkedboundry") {
        // std::cout << "3" << std::endl;
        Body::setBoundaryChunkedBody( body );
    }
    else if (this->bodyRequestType == "Content-Length") {
        // std::cout << "4" << std::endl;
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
        std::map<std::string, std::string>::iterator it = this->mime.find(this->header->getValue("Content-Type"));
        if ( it != this->mime.end()) {
            name += it->second;
        }
    }
}


void Body::resetAttributes (void) {
    std::vector<boundaryData_t>         tmp;
    this->bodyTrackingNumber = 0;
    this->isBodyInitiates = false;

    this->randomeChunkedName = "";
    this->randomeContentLengthName = "";
    this->restChunked = "";
    this->body = "";
    this->rest = "";
    this->bodyRequestType = "";
    this->data = tmp;
    this->contentLength = 0;
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
            this->restChunked = "";
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
