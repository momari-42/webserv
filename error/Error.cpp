/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:14:47 by momari            #+#    #+#             */
/*   Updated: 2025/02/12 15:31:28 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

//204 301 403 404 500 409


// Content-Type: text/html; charset=UTF-8
// Server: ECAcc (nyd/D13E)
// Content-Length: 1256
// Date: Fri, 28 Jun 2024 11:40:58 GMT
 
Error::Error ( int fd, std::string statusCode ) {
    std::string         line;
    std::ostringstream  contentLength;

    this->fd = fd;
    this->statusCode  = statusCode;
    this->httpVersion = "HTTP/1.1";
    this->description["204"] = "No Content";
    this->description["301"] = "Moved Permanently";
    this->description["400"] = "Bad Request";
    this->description["403"] = "Forbidden";
    this->description["404"] = "Not Found";
    this->description["409"] = "Conflict";
    this->description["500"] = "Internal Server Error";

        // initiate the header of the error response 

    this->header["Content-Type"] = "text/html; charset=UTF-8";
    this->header["Server"] = "momari-zaelarb";
    this->header["Connection"] = "close";

    std::fstream errorFile( "error/errorPages/" + this->statusCode + ".html");
    while (getline(errorFile, line)) {
        this->content += line + "\n";
    }
    contentLength << this->content.size();
    this->header["Content-Length"] = contentLength.str();
}

void Error::sendErrorPage ( void ) {
    std::string response;

    response += this->httpVersion + " " + this->statusCode + " " + this->description[this->statusCode] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += this->content;
    write(this->fd, response.c_str(), response.size());
}

Error::~Error ( void ) {
}

// std::string &Error::getDescription (void) {
//     return (this->description[this->errorCode]);
// }
