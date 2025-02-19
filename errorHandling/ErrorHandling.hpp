/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandling.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:57:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/19 11:46:19 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/sources.hpp"

class ErrorHandling : public std::exception
{
    private:
        std::string errorMsg;
    public:
        ErrorHandling( const std::string& errorMsg );
        ~ErrorHandling() throw() {
        };
        const char* what() const throw();
};