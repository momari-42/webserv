 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:19:44 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/09 15:03:51 by momari           ###   ########.fr       */
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

        // Reponse     response;
        // default  constructor and destructor
        Request( void );
        ~Request();
        void print( void );
        void parseRequest ( std::string requestData );
        RequestLine* getRequestLine();
        Header* getHeader();
        Body* getBody();

        
        void resetAttributes (void);

        std::string &getErrorCode();
        // this is just for test
        bool getBodyComplete( void);
};