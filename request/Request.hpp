/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:19:44 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/28 11:28:16 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "body/Body.hpp"
#include "header/Header.hpp"
#include "requestLine/RequestLine.hpp"

class Request
{
    private:
        RequestLine requestLine;
        Header      header;
        Body        body;
        int         trackingRequestNumber;
        
    public:
        Request( void );
        void print( void );
        void parseRequest ( std::string requestData );
        ~Request();
};