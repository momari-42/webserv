/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:46:20 by momari            #+#    #+#             */
/*   Updated: 2025/03/05 00:25:08 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "../include/sources.hpp"
#include "../client/Client.hpp"
#include "../error/Error.hpp"
#include "../configFile/ConfigFile.hpp"

// This is the buffer macro that i read from the client
#define BUFFER_SIZE 80000
#define NEVENTS     128

class Server
{
    private:
        
        // Config file for all the servers
        std::vector<ServerConfig> configs;
        
        // this variable is just for test how many request i get
        int                             numberOfRequest;


        
        // this is the attributte of the starting server;
        int                             kq;
        std::map<size_t, Client>        clients;
        std::string                     buffer;
        ssize_t                         bytesRead;
        struct kevent                   readyEvents[NEVENTS];
        size_t                          readyFd;
    
        socklen_t                       lenSocket;
        int                             sockfdClient;
        struct sockaddr_in              addressClient;
        std::vector<Socket>             sockets;

        std::map<size_t, ServerConfig>   srvs;
        std::map<size_t, size_t>        serverClientLinks;


        // this container is for erase the clients that receives the response
        std::vector<size_t>             clientsToErase;


        // this function check if sockfd exist in sockets container
        bool findFdSocket ( int sockfd );
        void checkServersConflict();
        bool checkSockets(std::string host, int port, ServerConfig* server);
    public:
        void startServer();
        Server ( std::string& config );
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