/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:18:55 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/28 13:46:34 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
// #include "../header/Header.hpp"
// #include "../Request.hpp"

// class Request;

typedef struct boundaryData {
    std::string name;
    std::string filename;
    std::string contenet;
} boundaryData_t;

class Body
{
    private:
        size_t      bodyTrackingNumber;
        std::string body;
        std::string rest;
        // Request *request;
        std::vector<boundaryData_t> data;
        void setChunkedBody( std::string& body, int &trackingRequestNumber );
        void setBoundaryBody( const std::string& requestData, const std::string& token );
        void setBoundaryChunkedBody( std::string& requestData, const std::string& token );
    public:
        Body( void );
        void printBody( void );
        void setBody( std::string& body, int &trackingRequestNumber, std::string token, std::string chunked );
        void parseBoundaryHeader(std::string& header);
        ~Body();
};