/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:46:20 by momari            #+#    #+#             */
/*   Updated: 2025/03/05 01:21:55 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include "../configFile/ConfigFile.hpp"
// #include "../request/Request.hpp"

class Socket
{
    private:
        std::string                 host;
        int                         port;
        int                         sockfd;
        struct sockaddr_in          addressServer;
        std::vector<ServerConfig*>  servers;
        int                         backlog;
        void socketBinding ();
        void socketListning ();
        void setSockOption ();

    public:
        Socket ( int port );
        ~Socket ( );
        void setServer(ServerConfig *server);
        int getPort();
        std::string& getHost();
        int getSockfd();
        ServerConfig* Socket::getServerConfig(std::string serverName);
        void initializeSocketCommunication ();

        // Exception class;

        class SocketExceptions : public std::exception
        {
            private:
                std::string errorMsg;
            public:
                SocketExceptions( const std::string& errorMsg );
                ~SocketExceptions() throw() {
                };
                const char* what() const throw();
        };
};
