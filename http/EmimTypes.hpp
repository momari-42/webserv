/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmimTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:06:08 by momari            #+#    #+#             */
/*   Updated: 2025/02/23 17:11:51 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

class EmimTypes
{
    protected:
        std::map<std::string, std::string>          emim;
    public:
        EmimTypes();
        ~EmimTypes();
};