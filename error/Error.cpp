/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:14:47 by momari            #+#    #+#             */
/*   Updated: 2025/03/16 22:44:29 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"
#define BUFFER_E 8000
//204 301 403 404 500 409


// Content-Type: text/html; charset=UTF-8
// Server: ECAcc (nyd/D13E)
// Content-Length: 1256
// Date: Fri, 28 Jun 2024 11:40:58 GMT
 
Error::Error ( int fd, std::string statusCode ) {
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

    std::fstream errorFile( "error/errorPages/" + this->statusCode + ".html");
    if (!errorFile.is_open()) {
        std::cout << "we are hirring error here " << std::endl;
    }
    while (true) {
        errorFile.read(buffer, BUFFER_E - 1);
        bytesRead = errorFile.gcount();
        buffer[bytesRead] = '\0';
        this->content.append(buffer, bytesRead);
        if (errorFile.eof())
            break;
    }
    contentLength << this->content.size();
    this->header["Content-Length"] = contentLength.str();
    errorFile.close();
}

void Error::sendErrorPage ( void ) {
    std::string response;

    // std::cout << "this is the content :" << this->content << std::endl;
    response += this->httpVersion + " " + this->statusCode + " " + this->statusCodes[this->statusCode] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += this->content;
    write(this->fd, response.c_str(), response.size());
    // std::cout << "we are here in the end of  class error" << std::endl;

}

Error::~Error ( void ) {
}

// std::string &Error::getDescription (void) {
//     return (this->description[this->errorCode]);
// }
