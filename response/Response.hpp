/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:08 by momari            #+#    #+#             */
/*   Updated: 2025/02/08 15:34:41 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/Request.hpp"

class Request;

class Response
{
    private:
        // RequestLine *requestLine;
        Request                                     *request;
        int                                         fd;
        std::string                                 statusCode;
        std::string                                 httpVersion;
        std::string                                 target;
        std::map<std::string, std::string>          description;
        std::map<std::string, std::string>          header;
        std::map<std::string, std::string>          mime;
        std::string                                 content;

    public:
        Response( int fd, Request *request );
        ~Response();
        void makeResponse ();
        void methodGet();
        void methodPost();
        void methodDelete();
        void setMime();
};