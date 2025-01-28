/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/28 17:49:53 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>

Body::Body( void ) {
    this->bodyTrackingNumber = 0;
    // this->request = request;
}

void Body::printBody( void ) {
    std::cout << this->rest << std::endl;
}

Body::~Body() {}

void Body::parseBoundaryHeader(std::string& header) {
    boundaryData_t part;
    part.name = header.substr(header.find("name=\"") + 6);
    part.name = part.name.substr(0, part.name.find("\""));
    std::cout << "The part.name is |" << part.name << "|" << std::endl;
    if (header.find("filename=\"") != std::string::npos) {
        part.filename = header.substr(header.find("filename=\"") + 10);
        part.filename = part.filename.substr(0, part.filename.find("\""));
        std::cout << "The part.filename is |" << part.filename << "|" << std::endl;
    }
    this->data.push_back(part);
}


void Body::setBoundaryBody( const std::string& requestData, const std::string& token ) {
    this->rest += requestData;
    if (this->rest.find(token) == 0 && this->rest.find(token, token.length()) != std::string::npos) {
        this->rest.erase(0, token.length() + 2);
        std::string header = this->rest.substr(0, this->rest.find("\r\n\r\n"));
        Body::parseBoundaryHeader(header);
        this->rest.erase(0, this->rest.find("\r\n\r\n") + 4);
        std::string body = this->rest.substr(0, this->rest.find(token) - 2);
        this->rest.erase(0, body.length() + 2);
        if (this->data.back().filename.length() > 0) {
            std::ofstream outputFile(this->data.back().filename, std::ios::binary);
            outputFile.write(body.data(), body.size());
            outputFile.close();
        } else
            this->data.back().contenet = body;
        if (this->rest.find(token) == this->rest.find(token + "--") && this->rest.find(token) == 0)
            this->rest.erase(0, token.size() + 2);
    }
}

void Body::setBoundaryChunkedBody( std::string& requestData, const std::string& token ) {
    // this->rest += requestData;
    // while (requestData.size()) {
    //     if ( !this->bodyTrackingNumber  && requestData.find("\r\n") != std::string::npos ) {
    //         this->bodyTrackingNumber = strtol(requestData.substr(0, requestData.find("\r\n")).c_str(), NULL, 16);
    //         requestData.erase(0, requestData.find("\r\n") + 2);
    //     }
    //     if (this->bodyTrackingNumber && this->bodyTrackingNumber <= requestData.size()) {
    //         this->rest.append(requestData, 0, this->bodyTrackingNumber);
    //         requestData.erase(0, this->bodyTrackingNumber);
    //         this->bodyTrackingNumber = 0;
    //     }
    //     else if ( this->bodyTrackingNumber ) {
    //         this->rest += requestData;
    //         this->bodyTrackingNumber -= requestData.size();
    //         requestData = "";
    //     }
    //     // std::cout << requestData.size() << " => " << this->bodyTrackingNumber << std::endl;
        
    //     // std::cout << "=============== before out =====================" << std::endl;
    // }
        // std::cout << "=============== out =====================" << std::endl;
    // std::ofstream before("before.txt", std::ios::binary);
    // before.write(this->rest.data(), this->rest.size());
    // before.close();
    // ======================================================================================
    (void) token;
    setChunkedBody( requestData );
    // if (this->bodyTrackingNumber == 0 && this->rest.size() == 0)
    //     setBoundaryBody(this->body, token);
}

void Body::setBody( std::string& body, int &trackingRequestNumber, std::string token, std::string chunked ) {
    (void) body;
    (void) trackingRequestNumber;
    if (token.find("boundary=") != std::string::npos && chunked.find("chunked") != std::string::npos)
        Body::setBoundaryChunkedBody(body, "--" + token.substr(token.find("boundary=") + 9));
    else if (token.find("boundary=") != std::string::npos)
        Body::setBoundaryBody(body, "--" + token.substr(token.find("boundary=") + 9));
    else
        Body::setChunkedBody(body);
}

void Body::setChunkedBody( std::string& body ) {
    this->rest += body;
    if ( !this->bodyTrackingNumber  && this->rest.find("\r\n") != std::string::npos ) {
        this->bodyTrackingNumber = strtol(this->rest.substr(0, this->rest.find("\r\n")).c_str(), NULL, 16);
        if (this->bodyTrackingNumber == 0 && this->rest.find("\r\n\r\n") != std::string::npos)
        {
            this->isBodyReceived = true;
            this->rest = "";
        }
        this->rest.erase(0, this->rest.find("\r\n") + 2);
    }
    if ( this->bodyTrackingNumber && this->rest.size() <= this->bodyTrackingNumber ) {
        this->body += this->rest;
        this->bodyTrackingNumber -= this->rest.size();
        this->rest = "";
    }
    else if ( this->bodyTrackingNumber ) {
        this->body += this->rest.substr(0, this->bodyTrackingNumber);
        this->rest.erase(0, this->bodyTrackingNumber + 2);
        this->bodyTrackingNumber = strtol(this->rest.substr(0, this->rest.find(";")).c_str(), NULL, 16);
        if (this->bodyTrackingNumber == 0 && this->rest.find("\r\n\r\n") != std::string::npos) {
            this->isBodyReceived = true;
            this->rest = "";
        }
        this->bodyTrackingNumber = 0;
    }
    body = "";
}
