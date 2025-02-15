/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:11 by momari            #+#    #+#             */
/*   Updated: 2025/02/10 20:33:17 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/Request.hpp"
#include "../response/Response.hpp"

class Client
{
    private:
        Request     request;
        Response    response;

    public:
        Client( );
        ~Client();
        Request &getRequest();
        Response &getResponse();
};