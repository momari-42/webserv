/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:08 by momari            #+#    #+#             */
/*   Updated: 2025/02/25 08:56:39 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/Request.hpp"
#include "../http/MimeTypes.hpp"
#include "../http/HttpResponse.hpp"
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

#define BUFFER_SIZE_R 8182

#define CRLF "\r\n"

// class Request;

class Response : public MimeTypes, public HttpResponse
{
    private:
        // this for error Code
        std::string                                 errorCode;
        // RequestLine *requestLine;
        Request                                     *request;
        std::string                                 statusCode;
        std::string                                 httpVersion;
        std::string                                 target;
        std::map<std::string, std::string>          description;
        std::map<std::string, std::string>          header;
        // std::map<std::string, std::string>          mime;

        std::fstream                                targetFile;
        bool                                        isHeaderSent;
        bool                                        isResponseSent;

        void validateAccessTarget( int fd, ConfigFile& configFile, std::string &requestTarget );
        void generateHeader ( int fd, std::string &response, ConfigFile& configFile );

    public:
        
        Response( Request *request );
        ~Response();
        void makeResponse ( size_t fd, ConfigFile& configFile  );
        void methodGet( size_t fd,   ConfigFile& configFile  );
        void methodPost( size_t fd );
        void executeCGI ( size_t fd );
        void methodDelete( size_t fd );
        bool getIsResponseSent();
        void resetAttributes();
        void setTargetFile();
        std::string &getErrorCode();
};