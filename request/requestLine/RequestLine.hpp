/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/07 20:16:53 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

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

        // RequestLine(const std::string &requestLine);

        ~RequestLine(); 
};
