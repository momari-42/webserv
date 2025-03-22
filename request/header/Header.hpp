/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:16:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/22 15:37:32 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../utils/utils.hpp"

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
        std::map<std::string, std::string> &getHttpHeadersMap( void );
        ~Header();

        void resetAttributes (void);
};
