/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:16:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/16 16:09:56 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

class Header
{
    private:
        std::map<std::string, std::string> httpHeadersMap;
        std::string header;
    public:
        Header(const std::string &header);
        ~Header();
};
