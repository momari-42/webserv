/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:18:55 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/30 14:45:40 by momari           ###   ########.fr       */
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

typedef struct boundaryData {
    std::string name;
    std::string filename;
    std::string contenet;
} boundaryData_t;

class Body
{
    private:
        // this just for test if request is  complete or not

        
        // this just for test
        bool                                requestComplete;
        // this is a boolean for tracking initialization of the body parameters
        bool                                isBodyInitiates;
        // this is a pointer to header request
        Header                              *header;


    

        ssize_t                             contentLength;
        
        // this is the data needed in body
        bool                                isBodyReceived;
        size_t                              bodyTrackingNumber;
        std::string                         body;
        std::string                         rest;
        std::string                         bodyRequestType;
        // std::map<std::string, std::string>  boundryData;
        // Request *request;
        std::vector<boundaryData_t> data;
        void setChunkedBody( std::string& body );
        void setBoundaryBody( const std::string& requestData, const std::string& token );
        void setBoundaryChunkedBody( std::string& requestData);
        void initiateBodyParams( void );
    public:
        Body( Header *header );
        void printBody( void );
        void setBody( std::string& body );
        void parseBoundaryHeader(std::string& header);
        bool getBodyComplete( void);
        ~Body();
};