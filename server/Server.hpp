/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:46:20 by momari            #+#    #+#             */
/*   Updated: 2025/01/29 11:03:31 by momari           ###   ########.fr       */
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
#include "../request/Request.hpp"
#include <vector>
#include "../socket/Socket.hpp"
#include <sys/event.h>
#include <fcntl.h>
#include <cstring>

// This is the buffer macro that i read from the client
#define BUFFER_SIZE 500

class Server
{
    private:
        socklen_t           lenSocket;
        int                 sockfdClient;
        struct sockaddr_in  addressClient;
        std::vector<Socket> sockets;
        // this function check if sockfd exist in sockets container
        bool findFdSocket ( int sockfd );

    public:
        void startServer();
        Server ( std::vector<int> vec );
        ~Server ( );
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
