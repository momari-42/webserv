/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:39:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/16 14:50:37 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( const std::string& request ) : requestLine(request), header(request), body(request) {
    // this->requestLine.setRequestLine(strtok((char *)request.c_str(), " "));
    // this->requestLine.setHeader(strtok((char *)request.c_str(), " "));
    // this->requestLine.setBody(request);
}

Request::~Request () {
    
}