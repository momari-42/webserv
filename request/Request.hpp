/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:19:44 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/08 11:22:35 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "body/Body.hpp"
#include "header/Header.hpp"
#include "requestLine/RequestLine.hpp"
#include "../response/Response.hpp"
#include "../error/Error.hpp"

class Request
{
    private:

        // if any error occure this integer will hold the error code for generate the respose of the error
        std::string         errorCode;
        // this objects is for manage parse of request and response

        RequestLine requestLine;
        Header      header;
        Body        body;

        //-----------------------------------------------------------

        int         trackingRequestNumber;
        bool        isRequestComplete;
        
    public:
        // default  constructor and destructor
        Request( void );
        ~Request();
        void print( void );
        void parseRequest ( std::string requestData );
        RequestLine* getRequestLine();
        Header* getHeader();
        Body* getBody();


        // this is just for test
        bool getBodyComplete( void);
};