/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:11 by momari            #+#    #+#             */
/*   Updated: 2025/02/19 20:34:32 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/Request.hpp"
#include "../response/Response.hpp"

class Request;

class Client
{
    private:
        Request     request;
        Response    response;

    public:
        Client();
        ~Client();
        Request &getRequest();
        Response &getResponse();
};