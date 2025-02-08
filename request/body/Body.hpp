/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:18:55 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/07 20:19:02 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "../header/Header.hpp"
// #include "../Request.hpp"

// class Request;

typedef struct  boundaryData {
    std::string name;
    std::string filename;
    std::string contenet;
    bool        isFile;
    bool        isHeaderComplete;
    bool        isBodyComplete;
}               boundaryData_t;

class Body
{
    private:

        std::string                         &errorCode;
        // this boolen is for request complete it point to a boolen in Reqeust class
        bool                                &isRequestComplete;
        
        // this just for test
        // bool                                requestComplete;
        // this is a boolean for tracking initialization of the body parameters
        bool                                isBodyInitiates;
        // this is a pointer to header request
        Header                              *header;


    

        ssize_t                             contentLength;

        // this is the data needed in body
        // bool                                isBodyReceived;
        size_t                              bodyTrackingNumber;
        std::string                         body;
        std::string                         rest;
        std::string                         restChunked;
        std::string                         bodyRequestType;
        // std::map<std::string, std::string>  boundryData;
        // Request *request;
        std::vector<boundaryData_t>         data;
        std::map<std::string, boundaryData_t>         boundryData;
        void setChunkedBody( std::string& body );
        void setBoundaryBody( std::string& requestData, const std::string& token );
        void setBoundaryChunkedBody( std::string& requestData);
        void initiateBodyParams( void );
    public:
        Body( Header *header, bool &isRequestComplete, std::string &errorCode );
        void printBody( void );
        void setBody( std::string& body );
        void parseBoundaryHeader(const std::string& header);
        ~Body();
};