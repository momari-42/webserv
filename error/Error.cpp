/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:14:47 by momari            #+#    #+#             */
/*   Updated: 2025/04/17 12:56:27 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"
#define BUFFER_E 8000

 
Error::Error ( int fd, std::string statusCode, std::map<std::string, std::string> errorPages ) {
    char                buffer[BUFFER_E];
    std::ostringstream  contentLength;
    size_t              bytesRead;

    this->fd = fd;
    this->statusCode  = statusCode;
    this->httpVersion = "HTTP/1.1";

    // initiate the header of the error response 

    this->header["Content-Type"] = "text/html; charset=UTF-8";
    this->header["Server"] = "momari-zaelarb";
    this->header["Connection"] = "close";

    std::fstream errorFile;
    if (errorPages.count(this->statusCode) && !access(errorPages[this->statusCode].c_str(), R_OK)) {
        errorFile.open(errorPages[this->statusCode]);
    }
    else {
        errorFile.open("error/errorPages/" + this->statusCode + ".html");
    }
    if (errorFile.is_open()) {
        while (true) {
            errorFile.read(buffer, BUFFER_E - 1);
            bytesRead = errorFile.gcount();
            buffer[bytesRead] = '\0';
            this->content.append(buffer, bytesRead);
            if (errorFile.eof())
                break;
        }
        errorFile.close();
    }
    else {
        this->content = "Error Code " + this->statusCode;
    }
    contentLength << this->content.size();
    this->header["Content-Length"] = contentLength.str();
}

void Error::sendErrorPage ( void ) {
    std::string response;

    response += this->httpVersion + " " + this->statusCode + " " + this->statusCodes[this->statusCode] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += this->content;
    if (send(this->fd, response.c_str(), response.size(), 0) == -1) {
        std::cerr << "Error: send() failed" << std::endl;
    }
}

Error::~Error ( void ) {
}