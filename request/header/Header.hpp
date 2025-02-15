/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:16:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/09 15:57:51 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <algorithm>

class Header
{
    private:
        std::string                         &errorCode;
        std::string                         rest;
        std::map<std::string, std::string>  httpHeadersMap;
    public:
        Header( std::string &errorCode );
        void setHeader( std::string &header, int& trackingRequestNumber );
        std::string getValue(const std::string& key);
        ~Header();

        void resetAttributes (void);
        // this just for debug
        void print( void );
};
