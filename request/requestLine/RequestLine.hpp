/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/24 10:45:50 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../include/sources.hpp"

class RequestLine
{
    private:
        bool        &isCgi;
        std::string &errorCode;
        std::string rest;
        std::string method;
        std::string requestTarget;
        std::string httpVersion;

        // additional methods
        void validateMethod( );
    public:
        // RequestLine();
        RequestLine( std::string  &errorCode, bool &isCgi );
        void setRequestLine( std::string& requestLine, int& trackingRequestNumber, ConfigFile& configFile  );
        std::string &getMethod ( void );
        std::string &getRequestTarget ( void );
        void resetAttributes (void);

        // RequestLine(const std::string &requestLine);

        ~RequestLine(); 
};
