/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:06 by momari            #+#    #+#             */
/*   Updated: 2025/02/15 13:16:19 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::setMime() {
    this->mime[".aac"]    = "audio/aac";
    this->mime[".abw"]    = "application/x-abiword";
    this->mime[".apng"]   = "image/apng";
    this->mime[".arc"]    = "application/x-freearc";
    this->mime[".avif"]   = "image/avif";
    this->mime[".avi"]    = "video/x-msvideo";
    this->mime[".azw"]    = "application/vnd.amazon.ebook";
    this->mime[".bin"]    = "application/octet-stream";
    this->mime[".bmp"]    = "image/bmp";
    this->mime[".bz"]     = "application/x-bzip";
    this->mime[".bz2"]    = "application/x-bzip2";
    this->mime[".cda"]    = "application/x-cdf";
    this->mime[".csh"]    = "application/x-csh";
    this->mime[".css"]    = "text/css";
    this->mime[".csv"]    = "text/csv";
    this->mime[".doc"]    = "application/msword";
    this->mime[".docx"]   = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    this->mime[".eot"]    = "application/vnd.ms-fontobject";
    this->mime[".epub"]   = "application/epub+zip";
    this->mime[".gz"]     = "application/gzip";
    this->mime[".gif"]    = "image/gif";
    this->mime[".htm"]    = "text/html";
    this->mime[".html"]   = "text/html";
    this->mime[".ico"]    = "image/vnd.microsoft.icon";
    this->mime[".ics"]    = "text/calendar";
    this->mime[".jar"]    = "application/java-archive";
    this->mime[".jpeg"]   = "image/jpeg";
    this->mime[".jpg"]    = "image/jpeg";
    this->mime[".js"]     = "text/javascript";
    this->mime[".json"]   = "application/json";
    this->mime[".jsonld"] = "application/ld+json";
    this->mime[".mid"]    = "audio/midi";
    this->mime[".midi"]   = "audio/midi";
    this->mime[".mjs"]    = "text/javascript";
    this->mime[".mp3"]    = "audio/mpeg";
    this->mime[".mp4"]    = "video/mp4";
    this->mime[".mpeg"]   = "video/mpeg";
    this->mime[".mpkg"]   = "application/vnd.apple.installer+xml";
    this->mime[".odp"]    = "application/vnd.oasis.opendocument.presentation";
    this->mime[".ods"]    = "application/vnd.oasis.opendocument.spreadsheet";
    this->mime[".odt"]    = "application/vnd.oasis.opendocument.text";
    this->mime[".oga"]    = "audio/ogg";
    this->mime[".ogv"]    = "video/ogg";
    this->mime[".ogx"]    = "application/ogg";
    this->mime[".opus"]   = "audio/ogg";
    this->mime[".otf"]    = "font/otf";
    this->mime[".png"]    = "image/png";
    this->mime[".pdf"]    = "application/pdf";
    this->mime[".php"]    = "application/x-httpd-php";
    this->mime[".ppt"]    = "application/vnd.ms-powerpoint";
    this->mime[".pptx"]   = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    this->mime[".rar"]    = "application/vnd.rar";
    this->mime[".rtf"]    = "application/rtf";
    this->mime[".sh"]     = "application/x-sh";
    this->mime[".svg"]    = "image/svg+xml";
    this->mime[".tar"]    = "application/x-tar";
    this->mime[".tif"]    = "image/tiff";
    this->mime[".tiff"]   = "image/tiff";
    this->mime[".ts"]     = "video/mp2t";
    this->mime[".ttf"]    = "font/ttf";
    this->mime[".txt"]    = "text/plain";
    this->mime[".vsd"]    = "application/vnd.visio";
    this->mime[".wav"]    = "audio/wav";
    this->mime[".weba"]   = "audio/webm";
    this->mime[".webm"]   = "video/webm";
    this->mime[".webp"]   = "image/webp";
    this->mime[".woff"]   = "font/woff";
    this->mime[".woff2"]  = "font/woff2";
    this->mime[".xhtml"]  = "application/xhtml+xml";
    this->mime[".xls"]    = "application/vnd.ms-excel";
    this->mime[".xlsx"]   = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    this->mime[".xml"]    = "application/xml";
    this->mime[".xul"]    = "application/vnd.mozilla.xul+xml";
    this->mime[".zip"]    = "application/zip";
    this->mime[".3gp"]    = "video/3gpp";
    this->mime[".3g2"]    = "video/3gpp2";
    this->mime[".7z"]     = "application/x-7z-compressed";
}

Response::Response ( Request *request  ) {
    this->isHeaderSent      = false;
    this->isResponseSent    = false;
    this->request = request;
    this->httpVersion = "HTTP/1.1";
    this->description["200"] = "OK";
    this->description["204"] = "No Content";
    this->description["201"] = "Created";
    Response::setMime();
    // this->header["Server"] = "momari-zaelarb";
    this->header["Connection"] = "keep-alive";
    this->header["Transfer-Encoding"] = "chunked";
    // std::cout << "i am here here : " << this->request->getRequestLine()->getRequestTarget() << std::endl;
}

Response::~Response (void) {
    
}

void Response::makeResponse ( size_t fd ) {
    if (this->request->getRequestLine()->getMethod() == "GET")
        methodGet( fd );
    else if (this->request->getRequestLine()->getMethod() == "POST")
        methodPost( fd );
    else if (this->request->getRequestLine()->getMethod() == "DELETE")
        methodDelete( fd );
}

std::string convertDecimalToHexaToString ( size_t number ) {
    std::ostringstream  stringNumberOne;

    stringNumberOne << std::hex << number;
    return (stringNumberOne.str());
}


void Response::methodGet( size_t fd ) {
    std::string         response;
    char                buffer[BUFFER_SIZE_R];
    std::streamsize     bytesRead;
    
    memset(buffer, 0, sizeof(buffer));
    if (!this->isHeaderSent) {
        if (this->request->getRequestLine()->getRequestTarget().find(".") != std::string::npos ) {
            std::map<std::string, std::string>::iterator it = this->mime.find( \
                this->request->getRequestLine()->getRequestTarget().substr(    \
                    this->request->getRequestLine()->getRequestTarget().find_last_of(".")));
            if (it != this->mime.end())
                this->header["Content-Type"] = it->second + "; charset=UTF-8";
        }
        response += this->httpVersion + " 200 " + this->description["200"] + CRLF;
        for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
            response += it->first + ": " + it->second + CRLF;
        response += CRLF;
        if (send(fd, response.c_str(), response.size(), 0) == -1) {
            std::cerr << "Error sending data" << std::endl;
            // Handle send error
        }
        setTargetFile();
        this->isHeaderSent = true;
        response.clear();
    }

    memset(buffer, 0, sizeof(buffer));
    targetFile.read(buffer, BUFFER_SIZE_R);
    bytesRead = targetFile.gcount();

    if (bytesRead > 0) {
        std::string content(buffer, bytesRead);
        std::string hexaNumber = convertDecimalToHexaToString(bytesRead); 
        response += hexaNumber + CRLF;
        response += content + CRLF;
        if (send(fd, response.c_str(), response.size(), 0) == -1) {
            std::cout << "Error sending data" << std::endl;
            // Handle send error
        }
    }

    if (targetFile.eof()) {
        response = "0" + std::string(CRLF) + CRLF;
        send(fd, response.c_str(), response.size(), 0);
        this->isResponseSent = true;
        targetFile.close(); // Close the file
    }
}
    // if (!this->isHeaderSent) {
    //     if (this->request->getRequestLine()->getRequestTarget().find(".") != std::string::npos ) {
    //         std::map<std::string, std::string>::iterator it = this->mime.find( \
    //             this->request->getRequestLine()->getRequestTarget().substr(    \
    //                 this->request->getRequestLine()->getRequestTarget().find_last_of(".")));
    //         if (it != this->mime.end())
    //             this->header["Content-Type"] = it->second + "; charset=UTF-8";
    //     }
    //     response += this->httpVersion + " 200 " + this->description["200"] + CRLF;
    //     for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
    //         response += it->first + ": " + it->second + CRLF;
    //     response += CRLF;
    //     setTargetFile();
    //     this->isHeaderSent = true;
    // }
    // targetFile.read(buffer, BUFFER_SIZE_R - 1);
    // std::streamsize bytesRead = targetFile.gcount();
    // std::string content(buffer, bytesRead);
    // std::string hexaNumber = convertDecimalToHexaToString(content.size());
    // response += hexaNumber + "\r\n";
    // response += content + "\r\n";
    // if ( targetFile.eof() ) {
    //     response += "0";
    //     response += "\r\n\r\n";
    //     send(fd, response.c_str(), response.size(), 0);
    //     this->isResponseSent = true;
    //     return;
    // }
    // send(fd, response.c_str(), response.size(), 0);
    // content = "";

std::string calculateBodyLength( std::string &body ) {
    std::ostringstream size;

    size << body.size();
    return (size.str());
}

void Response::methodPost( size_t fd ) {
    std::string response;
    std::string body = "File uploaded succefully !!";

    if (this->request->getHeader()->getValue("Connection") == "close") {
        this->header["Connection"] = "close";
    }
    this->header["Content-Length"] = calculateBodyLength( body );
    this->header["Content-Type"] = this->mime[".txt"];
    this->header.erase("Transfer-Encoding");
    response += this->httpVersion + " 201 " + this->description["201"] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += body;
    write(fd, response.c_str(), response.size());
    this->isResponseSent = true;
}

void Response::methodDelete( size_t fd ) {
    (void)fd;
}

bool Response::getIsResponseSent() {
    return (this->isResponseSent);
}

void Response::setTargetFile() {
    std::cout << "momari : " << this->request->getRequestLine()->getRequestTarget().c_str() << std::endl;
    this->targetFile.open(("." + this->request->getRequestLine()->getRequestTarget()).c_str(), std::ios::in);
}

void Response::resetAttributes() {
    this->isHeaderSent = false;
    this->isResponseSent = false;
    // targetFile.close();
}