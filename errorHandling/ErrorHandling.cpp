/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:57:28 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/19 10:57:40 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorHandling.hpp"

ErrorHandling::ErrorHandling ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

const char* ErrorHandling::what() const throw() {
    return (this->errorMsg.c_str());
}