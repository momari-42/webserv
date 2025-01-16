/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 11:19:44 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/16 14:50:41 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "body/Body.hpp"
#include "header/Header.hpp"
#include "requestLine/RequestLine.hpp"

class Request
{
    private:
        std::string request;
        RequestLine requestLine;
        Header      header;
        Body        body;
        
    public:
        Request(const std::string &request);
        // std::string getRequestLine() const;
        // std::string getHeader() const;
        // std::string getBody() const;
        ~Request();
};