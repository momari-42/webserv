/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/06 13:37:04 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../utils/utils.hpp"

class RequestLine
{
    private:
        bool        &isCgi;
        std::string &errorCode;
        std::string rest;
        std::string method;
        std::string requestTarget;
        std::string httpVersion;
        std::string tempraryRequestLine;

        // additional methods
        void validateMethod( );
    public:
        // RequestLine();
        RequestLine( std::string  &errorCode, bool &isCgi );
        void setRequestLine( std::string& requestLine, int& trackingRequestNumber );
        std::string &getMethod ( void );
        std::string &getRequestTarget ( void );
        std::string &getTempraryRequestLine ( void );
        void        resetAttributes (void);


        // RequestLine(const std::string &requestLine);

        ~RequestLine(); 
};
