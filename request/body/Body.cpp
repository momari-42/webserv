/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:39:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/06 11:45:35 by zaelarb          ###   ########.fr       */
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

bool Body::getBodyComplete( void) {
    return (this->requestComplete);
}



void Body::printBody( void ) {
//     std::cout << this->rest.size() << std::endl;
//     std::cout << "------------------------------------------------------------------------------------" << std::endl;
    // std::cout << this->body  << std::endl;
}

Body::~Body() {}

void Body::parseBoundaryHeader(const std::string& header) {
    boundaryData_t part;
    part.isFile = false;
    part.isHeaderComplete = false;
    part.isBodyComplete = false;
    std::cout << "<><start><>" << std::endl;
    std::cout << header << std::endl;
    std::cout << "<><end><>" << std::endl;
    part.name = header.substr(header.find("name=\"") + 6);
    part.name = part.name.substr(0, part.name.find("\""));
    if (header.find("filename=\"") != std::string::npos) {
        part.contenet = header.substr(header.find("filename=\"") + 10);
        part.contenet = part.contenet.substr(0, part.contenet.find("\""));
        part.isFile = true;
    }
    this->data.push_back(part);
}

// void Body::setBoundaryBody( const std::string& requestData, const std::string& token ) {
//     this->rest += requestData;
//     std::string body;
//     while (this->rest.size() > 0) {
//         if (this->rest.find(token) != std::string::npos)
//         {
//             if (this->rest.find(token) == this->rest.find(token + "--") && this->rest.find(token) == 0) {
//                 this->rest.erase(0);
//                 this->requestComplete = true;
//                 break;
//             }
//             else if (this->rest.find(token) != std::string::npos && this->rest.find(token) != this->rest.find(token + "--") && this->rest.find("\r\n\r\n") == std::string::npos)
//                 break;
//             if (this->rest.find(token) == 0 && this->rest.find("\r\n\r\n") != std::string::npos) {
//                 this->rest.erase(0, token.size() + 2);
//                 Body::parseBoundaryHeader(this->rest.substr(0, this->rest.find("\r\n\r\n")));
//                 this->rest.erase(0, this->rest.find("\r\n\r\n") + 4);
//             }
//             if (this->rest.find(token) != std::string::npos) {
//                 body = this->rest.substr(0, this->rest.find(token));
//                 this->rest.erase(0, body.size());
//             } else {
//                 body = this->rest.substr(0);
//                 this->rest.erase(0);
//             }
//         }
//         else {
//             body = this->rest.substr(0);
//             this->rest.erase(0, body.length() + 2);
//         }
//         if (this->data.back().isFile == true) {
//             std::ofstream outputFile( "upload/" + this->data.back().contenet, std::ios::binary | std::ios::app);
//             outputFile.write(body.data(), body.size());
//         } else
//             this->data.back().contenet = body;
//     }
//     body = "";
// }

void manageFile(const std::string fileName, const std::string data ) {
    std::ofstream outputFile( "upload/" + fileName, std::ios::binary | std::ios::app);
    outputFile.write(data.data(), data.size());
}

void Body::setBoundaryBody( std::string& requestData, const std::string& token ) {
    this->rest += requestData;

    while (this->rest.size() && !this->requestComplete) {
        // std::cout << "0000000000" << std::endl;


        if (this->rest.find(token + "--") != std::string::npos && this->rest.find(token) == this->rest.find(token + "--")) {
            // std::cout << "1" << std::endl;


            if (!this->data.back().isBodyComplete)
                manageFile(this->data.back().contenet, this->rest.substr(0, this->rest.find(token + "--") - 2));
            // manageFile("hihhihihihhi.py", this->rest);
            this->requestComplete = true;
            this->rest = "";
            break;
        }
        if (this->rest.find(token) != std::string::npos && this->rest.find("\r\n\r\n") != std::string::npos) {
            // std::cout << "2 " << std::endl;


            if (this->rest.find(token) != 0) {
                // std::cout << "3 " << std::endl;


                manageFile(this->data.back().contenet, this->rest.substr(0, this->rest.find(token) - 2));
                this->rest.erase(0, this->rest.find(token));
                this->data.back().isBodyComplete = true;
            }
            if (this->rest.find(token) == 0 && this->rest.find("\r\n\r\n") != std::string::npos) {
                // std::cout << "4 " << std::endl;


                this->rest.erase(0, token.size() + 2);
                Body::parseBoundaryHeader(this->rest.substr(0, this->rest.find("\r\n\r\n")));
                this->data.back().isHeaderComplete = true;
                std::cout << this->data.back().contenet << std::endl;


                this->rest.erase(0, this->rest.find("\r\n\r\n") + 4);
            }
        }
        else if (this->data.size() && this->data.back().isHeaderComplete && !this->data.back().isBodyComplete) {
            // std::cout << "5 " << std::endl;
            if ( (this->rest.find_last_of(token) == std::string::npos) || ( this->rest.size() - this->rest.find_last_of(token) > token.size() )) {
                // std::cout << "6 " << std::endl;
                manageFile(this->data.back().contenet, this->rest); 
                this->rest = "";
                break;
            }
            else {
                // std::cout << "7 " << std::endl;
                break;
            }
        }
        else
        
            break;
    }
    requestData = "";
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
        
        // std::cout << "=============== before out =====================" << std::endl;
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
    Body::setBoundaryBody(this->body, "--" + this->header->getValue("Content-Type").substr(this->header->getValue("Content-Type").find("boundary=") + 9));
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
    this->restChunked += body;
    // std::cout << this->restChunked << std::endl;
    while ( this->restChunked.size() )
    {
        if ( !this->bodyTrackingNumber  && this->restChunked.find("\r\n") == std::string::npos )
            break;
        if ( !this->bodyTrackingNumber  && this->restChunked.find("\r\n") != std::string::npos ) {
            this->bodyTrackingNumber = strtol(this->restChunked.substr(0, this->restChunked.find("\r\n")).c_str(), NULL, 16);
            this->restChunked.erase(0, this->restChunked.find("\r\n") + 2);
            if (this->bodyTrackingNumber == 0) {
                this->isBodyReceived = true;
                // std::cerr << "-------------***-------------" << std::endl;

                // in this i will open a test file for store  the value that i parse just for test
                // manageFile("screenTest.png", this->body);

                this->restChunked = "";
                break;
            }
        }
        if ( this->bodyTrackingNumber && this->restChunked.size() <= this->bodyTrackingNumber ) {
            this->body += this->restChunked;
            this->bodyTrackingNumber -= this->restChunked.size();
            this->restChunked = "";
        }
        else if ( this->bodyTrackingNumber ) {
            this->body += this->restChunked.substr(0, this->bodyTrackingNumber);
            this->restChunked.erase(0, this->bodyTrackingNumber + 2);
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
