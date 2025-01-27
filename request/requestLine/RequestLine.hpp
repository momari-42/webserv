/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/26 21:27:27 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class RequestLine
{
    private:
        std::string rest;
        std::string method;
        std::string requestTarget;
        std::string httpVersion;
    public:
        // RequestLine();
        RequestLine( void );
        void setRequestLine( std::string& requestLine, int& trackingRequestNumber );

        void printFirstLine( void );
        // RequestLine(const std::string &requestLine);
        ~RequestLine(); 
};
