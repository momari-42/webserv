/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/13 10:14:34 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>

class RequestLine
{
    private:
        std::string &errorCode;
        std::string rest;
        std::string method;
        std::string requestTarget;
        std::string httpVersion;
    public:
        // RequestLine();
        RequestLine( std::string  &errorCode );
        void setRequestLine( std::string& requestLine, int& trackingRequestNumber );
        std::string &getMethod ( void );
        std::string &getRequestTarget ( void );
        void resetAttributes (void);

        // RequestLine(const std::string &requestLine);

        ~RequestLine(); 
};
