/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:41:11 by momari            #+#    #+#             */
/*   Updated: 2025/01/26 19:50:01 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
#include "request/requestLine/RequestLine.hpp"

// void sigHandler( int signal ) {
//     try {
//         if ( signal == SIGINT ) {
//             std::cout << "here" << std::endl;
//             throw("Close Server");
//         }   
//     } catch ( const char* message ) {
//         std::cout << message << std::endl;
//     }
// }

int main() {
    try {
        std::vector<int> vec;
        vec.push_back(9998);
        // vec.push_back(a);
        Server server(vec);
        server.startServer();
        // signal(SIGINT, server.sigHandler);
        // server.runServer ();
    }
    catch ( std::exception& e ) {
        std::cout << "exception" << std::endl;
        std::cout << e.what() << std::endl;
        return (1);
    }
}