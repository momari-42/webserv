/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:14:50 by momari            #+#    #+#             */
/*   Updated: 2025/03/19 15:24:24 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../http/HttpResponse.hpp"
#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sstream>

#define CRLF "\r\n"

class Error : public HttpResponse
{
    private:
        int                                         fd;
        std::string                                 statusCode;
        std::string                                 httpVersion;
        std::map<std::string, std::string>          header;
        std::string                                 content;
        // size_t                                      errorLength;

    public:
        void sendErrorPage ( void );
        Error(  int fd, std::string statusCode, std::map<std::string, std::string> errorPages );
        ~Error();
};