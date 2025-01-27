/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:18:55 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/27 12:55:16 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class Body
{
    private:
        size_t      bodyTrackingNumber;
        std::string body;
        std::string rest;
        void setChunkedBody( std::string& body, int &trackingRequestNumber );
    public:
        Body( void );
        void printBody( void );
        void setBody( std::string& body, int &trackingRequestNumber );
        ~Body();
};