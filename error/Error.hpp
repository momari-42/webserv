/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:14:50 by momari            #+#    #+#             */
/*   Updated: 2025/02/07 22:10:16 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <cstring>
#include <unistd.h>

#define CRLF "\r\n"

class Error
{
    private:
        int                                         fd;
        std::string                                 statusCode;
        std::string                                 httpVersion;
        std::map<std::string, std::string>          description;
        std::map<std::string, std::string>          header;
        std::string                                 content;
        // size_t                                      contentlength;

    public:
        void sendErrorPage ( void );
        Error(  int fd, std::string statusCode );
        ~Error();
};