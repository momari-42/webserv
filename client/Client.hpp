/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:11 by momari            #+#    #+#             */
/*   Updated: 2025/03/18 10:55:27 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "../response/Response.hpp"
#include "../request/Request.hpp"
#include "../socket/Socket.hpp"

class Response;

class Client
{
    private:
        Request     request;
        Response    response;
        Socket*     socket;
        bool        isConfigFileInitialized;
        std::string sessionID;

    public:
        Client();
        ~Client();
        void setConfig(size_t fd, std::vector<Socket>& sockets);
        Request &getRequest();
        Response &getResponse();
};