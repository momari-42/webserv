/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:16:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/27 15:28:15 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <algorithm>

class Header
{
    private:
        std::string                         rest;
        std::map<std::string, std::string>  httpHeadersMap;
    public:
        Header( void );
        void setHeader( std::string &header, int& trackingRequestNumber );
        std::string getValue(const std::string& key);
        ~Header();

        
        // this just for debug
        void print( void );
};
