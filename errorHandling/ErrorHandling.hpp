/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandling.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:57:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/19 21:12:53 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <iostream>

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