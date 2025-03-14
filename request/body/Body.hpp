/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:18:55 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/12 23:50:00 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../utils/utils.hpp"
#include "../header/Header.hpp"
#include "../../http/EmimTypes.hpp"
#include "../../configFile/ConfigFile.hpp"
#include <fstream>
#include <dirent.h>
// class Request;

typedef struct  boundaryData {
    std::string name;
    std::string filename;
    std::string contenet;
    bool        isFile;
    bool        isHeaderComplete;
    bool        isBodyComplete;
}               boundaryData_t;

class Body : public EmimTypes
{
    private:

        std::string                         &errorCode;
        std::string                         requestTarget;
        // this boolen is for request complete it point to a boolen in Reqeust class
        bool                                &isRequestComplete;
        
        // this just for test
        // bool                                requestComplete;
        // this is a boolean for tracking initialization of the body parameters
        bool                                isBodyInitiates;
        // this is a pointer to header request
        Header                              *header;

        size_t                             bodyLength;
        // this attributes is for content length parse
        size_t                             contentLength;
        std::string                        randomeContentLengthName;
    


        std::string                         randomeFileName;
        std::string                         restChunked;
        std::string                         body;

        size_t                              bodyTrackingNumber;
        std::string                         rest;
        std::string                         bodyRequestType;

        std::vector<boundaryData_t>         data;

        ServerConfig                        *configFile;
        
        // this variables are all for cgi http request
        bool                                cgi;
        bool                                isShunked;
        // std::map<std::string, std::string>         mime;
        
        void generateRandomeName( std::string& name );
        void setChunkedBody( std::string& body );
        void setBoundaryBody( std::string& requestData, const std::string& token );
        void setBoundaryChunkedBody( std::string& requestData);
        void setContentLengthBody( std::string& requestData );
        void setCgi( std::string& requestData );
        void setChunkedCgiBody( std::string& requestData );
        void initiateBodyParams( void );
        void validateFileName( void );
    public:
        Body( Header *header, bool &isRequestComplete, std::string &errorCode );
        void printBody( void );
        void setBody( std::string& body, bool &cgi );
        void parseBoundaryHeader(const std::string& header);

        void resetAttributes (void);
        std::string &getRandomeFileName();
        void setConfigFile(ServerConfig* configFile);
        void setRequestTarget(std::string &requestTarget);
        void checkAccess( std::string &requestTarget );
        void manageFile(const std::string fileName, const std::string data );
        std::string getBodyRequestType();
        size_t getBodyLength();
        
        ~Body();
};