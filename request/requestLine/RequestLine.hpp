/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:10:34 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/25 15:33:08 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class RequestLine
{
    private:
        std::string requestLine;
        std::string method;
        std::string requestTarget;
        std::string httpVersion;
    public:
        // RequestLine();
        RequestLine( void );
        void setRequestLine( const std::string& requestLine );
        // RequestLine(const std::string &requestLine);
        ~RequestLine(); 
};
