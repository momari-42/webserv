 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:19:44 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/09 15:03:51 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requestLine/RequestLine.hpp"
#include "body/Body.hpp"
#include "header/Header.hpp"
#include "../configFile/ConfigFile.hpp"
#include "../socket/Socket.hpp"
#include <map>

class Request
{
    private:

        std::string         path;
        std::string         root;
        Location            location;
        std::string         requestTarget;
        // this is for initiate the config file and validate request line
        bool                checkRequestLine;
        // if any error occure this integer will hold the error code for generate the respose of the error
        std::string         errorCode;
        // this objects is for manage parse of request and response

        RequestLine         requestLine;
        Header              header;
        Body                body;
        ServerConfig        *configFile;

        //-----------------------------------------------------------

        int                 trackingRequestNumber;
        bool                isRequestComplete;
        bool                isCgi;
        
        Socket              *socket;

    public:
        // Reponse     response;
        // default  constructor and destructor
        Request( );
        ~Request();
        void print( void );
        void parseRequest ( std::string requestData );
        RequestLine* getRequestLine();
        Header* getHeader();
        Body* getBody();

        
        void resetAttributes (void);
        void setSocket( Socket *socket );

        std::string &getErrorCode();
        // this is just for test
        bool getBodyComplete( void);
        bool getIsCgi( void);
        size_t getTrackingRequestNumber( void );
        void setConfigFile(ServerConfig* configFile);
        std::string &getFileName();
        void validateMethod(std::string &method, std::vector<std::string> &methods);
        std::string &getPath();
        std::string &getRoot();
        std::string &getRequestTarget();
        Location &getLocation();
};