/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:41:11 by momari            #+#    #+#             */
/*   Updated: 2025/02/14 17:42:07 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"
// #include "request/requestLine/RequestLine.hpp"

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
    signal(SIGPIPE, SIG_IGN);

    try {
        std::vector<int> vec;
        vec.push_back(9999);
        Server server(vec);
        server.startServer();
    }
    catch ( std::exception& e ) {
        std::cout << "exception" << std::endl;
        std::cout << e.what() << std::endl;
        return (1);
    }
}