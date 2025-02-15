/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:08 by momari            #+#    #+#             */
/*   Updated: 2025/02/15 13:06:01 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/Request.hpp"
#include <cstring>
#include <sstream>
#include <sys/socket.h>
 #include <sys/types.h>

#define BUFFER_SIZE_R 8182

class Request;

class Response
{
    private:
        // RequestLine *requestLine;
        Request                                     *request;
        std::string                                 statusCode;
        std::string                                 httpVersion;
        std::string                                 target;
        std::map<std::string, std::string>          description;
        std::map<std::string, std::string>          header;
        std::map<std::string, std::string>          mime;

        std::fstream                                targetFile;
        bool                                        isHeaderSent;
        bool                                        isResponseSent;

    public:
        
        Response( Request *request );
        ~Response();
        void makeResponse ( size_t fd );
        void methodGet( size_t fd );
        void methodPost( size_t fd );
        void methodDelete( size_t fd );
        void setMime();
        bool getIsResponseSent();
        void resetAttributes();
        // Response( const Response& response) {
        //     *this = response;
        // }
        // Response &operator=( const Response& response) {
        //     if (this != &response)
        //     {
        //             request = response.request;
        //             // statusCode = response.statusCode;
        //             httpVersion = response.httpVersion;
        //             target = response.target;
        //             description = response.description;
        //             header = response.header;
        //             mime = response.mime;
        //             content = response.content;
        //             // targetFile = response.targetFile;
        //             isHeaderSent = response.isHeaderSent;
        //             isResponseSent = response.isResponseSent;
        //     }
        //     return (*this);
        // }
        void setTargetFile();
};