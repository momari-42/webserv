/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:12:25 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/16 14:57:00 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestLine.hpp"

RequestLine::RequestLine(const std::string &requestLine) {
    this->method = strtok((char *)requestLine.c_str(), " ");
    this->requestTarget = strtok(NULL, " ");
    this->requestTarget = strtok(NULL, " ");
    // this->httpVersion = requestLine;

    std::cout << "Method : " << this->method << std::endl;
    std::cout << "Request Target : " << this->requestTarget << std::endl;
    std::cout << "Http Version : " << this->httpVersion << std::endl;
}

RequestLine::~RequestLine() {}