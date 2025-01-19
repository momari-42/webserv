/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:46:20 by momari            #+#    #+#             */
/*   Updated: 2025/01/18 19:44:44 by momari           ###   ########.fr       */
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

class Socket
{
    private:
        int                 sockfd;
        int                 backlog;
        struct sockaddr_in  addressServer;
        void socketBinding ();
        void socketListning ();
        // void socketAccepting ();
        // void receiveRequest ();
        void setSockOption ();

    public:
        Socket ( int port );
        ~Socket ( );
        int getSockfd();
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
