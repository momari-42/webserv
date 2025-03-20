/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:11 by momari            #+#    #+#             */
/*   Updated: 2025/03/19 01:40:45 by momari           ###   ########.fr       */
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
        bool        isReadyForNextRequest;
        // size_t      ident;
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
        // size_t getIdent();
        bool getIsReadyForNextRequest();
        // void setIdent( size_t ident );
};