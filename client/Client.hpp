/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:11 by momari            #+#    #+#             */
/*   Updated: 2025/03/05 00:41:15 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/Request.hpp"
#include "../response/Response.hpp"
#include "../socket/Socket.hpp"

class Client
{
    private:
        Request     request;
        Response    response;
        Socket*     socket;

    public:
        Client();
        ~Client();
        void setConfig(size_t fd, std::vector<Socket>);
        Request &getRequest();
        Response &getResponse();
};