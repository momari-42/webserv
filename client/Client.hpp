/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:47:11 by momari            #+#    #+#             */
/*   Updated: 2025/03/06 15:34:17 by momari           ###   ########.fr       */
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
        bool        isConfigFileInitialized;
        std::string name;

    public:
        Client();
        ~Client();
        void setConfig(size_t fd, std::vector<Socket>& sockets);
        Request &getRequest();
        Response &getResponse();
};