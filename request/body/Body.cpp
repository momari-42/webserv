/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/30 14:45:42 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Body.hpp"
#include <unistd.h>

Body::Body( Header *header ) {
    this->header = header;
    this->bodyTrackingNumber = 0;
    this->isBodyInitiates = false;
    this->requestComplete  = false;
    
    // this->request = request;
}

void Body::printBody( void ) {
//     std::cout << this->rest.size() << std::endl;
//     std::cout << "------------------------------------------------------------------------------------" << std::endl;
    // std::cout << this->body  << std::endl;
}

Body::~Body() {}

bool Body::getBodyComplete( void) {
    return (this->requestComplete);
}


void Body::parseBoundaryHeader(std::string& header) {
    boundaryData_t part;
    part.name = header.substr(header.find("name=\"") + 6);
    part.name = part.name.substr(0, part.name.find("\""));
    // std::cout << "The part.name is |" << part.name << "|" << std::endl;
    if (header.find("filename=\"") != std::string::npos) {
        part.filename = header.substr(header.find("filename=\"") + 10);
        part.filename = part.filename.substr(0, part.filename.find("\""));
        this->requestComplete = true;
        // std::cout << "The part.filename is |" << part.filename << "|" << std::endl;
    }
    this->data.push_back(part);
}


void Body::setBoundaryBody( const std::string& requestData, const std::string& token ) {
    this->rest += requestData;
    // std::cout << "I am in the boundary function" << std::endl;
    // std::cout << "==============================================================" << std::endl;
    // std::cout << this->rest;
    // std::cout << "==============================================================" << std::endl;
    while (this->rest.find(token) == 0 && this->rest.find(token, token.length()) != std::string::npos) {
        // std::cerr << "Boundary" << std::endl;
        this->rest.erase(0, token.length() + 2);
        std::string header = this->rest.substr(0, this->rest.find("\r\n\r\n"));
        Body::parseBoundaryHeader(header);
        this->rest.erase(0, this->rest.find("\r\n\r\n") + 4);
        std::string body = this->rest.substr(0, this->rest.find(token) - 2);
        this->rest.erase(0, body.length() + 2);
        if (this->data.back().filename.length() > 0) {
            std::ofstream outputFile(this->data.back().filename, std::ios::binary);
            outputFile.write(body.data(), body.size());
            
            // outputFile.close();
        } else
            this->data.back().contenet = body;
        if (this->rest.find(token) == this->rest.find(token + "--") && this->rest.find(token) == 0)
            this->rest.erase(0, token.size() + 2);
    }
}

void Body::setBoundaryChunkedBody( std::string& body) {
    // std::cerr << "I get in =================" << std::endl;
    // std::cerr << body << std::endl;
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
    // (void) token;
    // setChunkedBody( requestData );
    // if (this->bodyTrackingNumber == 0 && this->rest.size() == 0)
    //     setBoundaryBody(this->body, token);
    // if (this->isBodyReceived == false)
        Body::setChunkedBody(body);
    if (this->isBodyReceived == true) {
        // std::cout << this->body;
        // this->rest = "";
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        // std::cout << "===================================================================================" << std::endl;
        Body::setBoundaryBody(this->body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
    }
    // if (this->isBodyReceived){
    //     std::cout << this->body;
    //     exit (1);
    // }
    // (void)body;
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
        this->bodyRequestType = "contentlength";
    }
}

void Body::setBody( std::string& body ) {
    // (void) body;
    // (void) chunked;
    // (void) trackingRequestNumber;
    // if (token.find("boundary=") != std::string::npos && chunked.find("chunked") != std::string::npos)
    //     Body::setBoundaryChunkedBody(body, "--" + token.substr(token.find("boundary=") + 9));
    // else if (token.find("boundary=") != std::string::npos)
    //     Body::setBoundaryBody(body, "--" + token.substr(token.find("boundary=") + 9));
    // else
    
    
    if (!this->isBodyInitiates) {
        initiateBodyParams();
        this->isBodyInitiates = true;
    }
    if (this->bodyRequestType == "chunked")
        Body::setChunkedBody(body);
    else if (this->bodyRequestType == "boundry")
        Body::setBoundaryBody(body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
    else if (this->bodyRequestType == "chunkedboundry")
    {
        // std::cerr << "lol" << std::endl;
        Body::setBoundaryChunkedBody( body );
    }
}

void Body::setChunkedBody( std::string& body ) {
    // std::cout << "I am in with no one where are you guys " << std::endl;
    this->rest += body;
    // std::cout << this->rest << std::endl;
    while ( this->rest.size() )
    {
        if ( !this->bodyTrackingNumber  && this->rest.find("\r\n") == std::string::npos )
            break;
        if ( !this->bodyTrackingNumber  && this->rest.find("\r\n") != std::string::npos ) {
            this->bodyTrackingNumber = strtol(this->rest.substr(0, this->rest.find("\r\n")).c_str(), NULL, 16);
            this->rest.erase(0, this->rest.find("\r\n") + 2);
            if (this->bodyTrackingNumber == 0) {
                this->isBodyReceived = true;
                std::cerr << "-------------***-------------" << std::endl;
                this->rest = "";
                break;
            }
        }
        if ( this->bodyTrackingNumber && this->rest.size() <= this->bodyTrackingNumber ) {
            this->body += this->rest;
            this->bodyTrackingNumber -= this->rest.size();
            this->rest = "";
        }
        else if ( this->bodyTrackingNumber ) {
            this->body += this->rest.substr(0, this->bodyTrackingNumber);
            this->rest.erase(0, this->bodyTrackingNumber + 2);
            this->bodyTrackingNumber = 0;
        }
        // if ( !this->bodyTrackingNumber && this->rest.find("\r\n\r\n") == 0 ) {
        //     this->isBodyReceived = true;
        //     std::cout << this->body;
        //     this->rest = "";
        //     exit(1);
        // }
        // std::cout << "In the while \n";
    }
    
    body = "";
}
