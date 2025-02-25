/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:03:35 by momari            #+#    #+#             */
/*   Updated: 2025/02/23 17:17:46 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

class HttpResponse
{
    protected:
        std::map<std::string, std::string> statusCodes;
    public:
        HttpResponse(/* args */);
        ~HttpResponse();
};