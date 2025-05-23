/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/22 22:23:44 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../utils/utils.hpp"
#include <sstream>

class RequestLine
{
    private:
        std::string &errorCode;
        std::string rest;
        std::string method;
        std::string requestTarget;
        std::string httpVersion;
        std::string path;
        std::string data;
        std::string tempraryRequestLine;

        void validateMethod( );
    public:
        RequestLine( std::string  &errorCode );
        void setRequestLine( std::string& requestLine, int& trackingRequestNumber );
        std::string &getMethod ( void );
        std::string &getRequestTarget ( void );
        std::string &getPath ( void );
        std::string &getData ( void );
        std::string &getTempraryRequestLine ( void );
        void        resetAttributes (void);

        ~RequestLine(); 
};
