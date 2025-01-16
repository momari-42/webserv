/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:46:20 by momari            #+#    #+#             */
/*   Updated: 2025/01/16 11:12:23 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../webserver.hpp"

class Server
{
    private:
        socklen_t           client_len;
        int                 sockfdServer;
        int                 sockfdClient;
        int                 backlog;
        struct sockaddr_in  addressServer;
        struct sockaddr_in  addressClient;
        std::string         request;
        void serverBinding ();
        void serverListning ();
        void serverAccepting ();
        void receiveRequest ();
        void setSockOption ();

    public:
        Server (int port);
        ~Server ( );
        void runServer ();
        // Exception class;
        class ServerExceptions : public std::exception
        {
            private:
                std::string errorMsg;
            public:
                ServerExceptions( const std::string& errorMsg );
                ~ServerExceptions() throw() {
                };
                const char* what() const throw();
        };
};
