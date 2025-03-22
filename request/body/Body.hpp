/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:18:55 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/22 15:37:24 by momari           ###   ########.fr       */
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
    std::string content;
    bool        isFile;
    bool        isHeaderComplete;
    bool        isBodyComplete;
}               boundaryData_t;

class Body : public EmimTypes
{
    private:
        std::string                         nextRequest;
        std::string                         &errorCode;
        std::string                         requestTarget;
        // this boolen is for request complete it point to a boolen in Reqeust class
        bool                                &isRequestComplete;
        bool                                created;
        
        // this just for test
        // bool                                requestComplete;
        // this is a boolean for tracking initialization of the body parameters
        bool                                isBodyInitiates;
        // this is a pointer to header request
        Header                              *header;

        ssize_t                             bodyLength;
        // this attributes is for content length parse
        ssize_t                             contentLength;
        std::string                        randomeContentLengthName;
    


        std::string                         randomeFileName;
        std::string                         restChunked;
        std::string                         body;

        size_t                              bodyTrackingNumber;
        std::string                         rest;
        std::string                         bodyRequestType;

        std::vector<boundaryData_t>         data;

        ServerConfig                        *configFile;
        
        std::string                         method;
        // this variables are all for cgi http request
        bool                                cgi;
        bool                                isShunked;
        // std::map<std::string, std::string>         mime;
        std::string                         boundryToken;
        
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
        void setBody( std::string& body, bool &cgi, std::string &method );
        void parseBoundaryHeader(const std::string& header);

        void resetAttributes (void);
        std::string &getRandomeFileName();
        void setConfigFile(ServerConfig* configFile);
        void setRequestTarget(std::string &requestTarget);
        void checkAccess( std::string &requestTarget );
        void manageFile(const std::string fileName, const std::string data );
        void manageExistinceFile(std::string &fileName);
        std::string getBodyRequestType();
        std::string &getNextRequest();
        void setNextRequest( std::string nextRequest );
        size_t getBodyLength();
        bool getCreated();
        void unlinkCreatedFiles( std::vector<boundaryData_t> &data );
        
        ~Body();
};