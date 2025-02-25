/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:03:40 by momari            #+#    #+#             */
/*   Updated: 2025/02/23 17:08:01 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

class MimeTypes
{
    protected:
        std::map<std::string, std::string>          mime;
    public:
        MimeTypes();
        ~MimeTypes();
};