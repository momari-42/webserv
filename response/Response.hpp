/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:08 by momari            #+#    #+#             */
/*   Updated: 2025/03/22 22:24:39 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../configFile/ConfigFile.hpp"
#include "../request/Request.hpp"
#include "../http/MimeTypes.hpp"
#include "../http/HttpResponse.hpp"
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE_R 16384

#define CRLF "\r\n"

// class Request;

class Response : public MimeTypes, public HttpResponse
{
    private:

        std::string                                 queryString;

        bool                                        &isReadyForNextRequest;
    
        Socket*                                     socket;
        ServerConfig*                               configFile;
        Location                                    location;
        bool                                        initiatConfigFile;

        std::string                                 sessionID;
        std::map<std::string, std::string>          cookies;
        bool                                        listingDirectory;
        std::string                                 folderName;
        // this for error Code
        std::string                                 errorCode;
        // RequestLine *requestLine;
        Request                                     *request;
        std::string                                 statusCode;
        std::string                                 httpVersion;
        std::string                                 target;
        std::map<std::string, std::string>          description;
        std::map<std::string, std::string>          header;

        std::fstream                                targetFile;
        bool                                        isHeaderSent;
        bool                                        isResponseSent;

        void validateAccessTarget( int fd, std::string &requestTarget );
        void generateHeader ( int fd, std::string &response );

        // this variables is for cgi execution
        int                                         pid;
        bool                                        isCgiComplet;
        int                                         fd[2];
        int                                         inout[2];
        int                                         fdClient;

    public:
        
        Response( Request *request, bool &isReadyForNextRequest );
        ~Response();
        void makeResponse ( size_t fd, size_t kq );
        void methodGet( size_t fd );
        void methodPost( size_t fd );
        void executeCGI (  size_t fd, size_t kq );
        void methodDelete( size_t fd );
        bool getIsResponseSent();
        void resetAttributes();
        void setTargetFile();
        void validateRequestTarget();
        std::string &getErrorCode();
        void setConfigFile(ServerConfig* configFile);
        void setSocket( Socket *socket );
        void sendSuccessResponse( size_t fd );
        void sendNoContentResponse( size_t fd );
        void setServerCookies();
        void sendDirectoryList( size_t fd );

        void setIsReadyForNextRequest ( bool isReadyForNextRequest );

        void sendRedirectionResponse( size_t fd, Location &location );
        class ResponseExceptions : public std::exception
        {
            private:
                std::string errorMsg;
            public:
                ResponseExceptions( const std::string& errorMsg );
                ~ResponseExceptions() throw() {
                };
                const char* what() const throw();
        };
};

bool isDirectory(std::string &path);