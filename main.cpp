/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:41:11 by momari            #+#    #+#             */
/*   Updated: 2025/01/11 10:12:58 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserver.hpp"

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
        Server server(12346);
        // signal(SIGINT, server.sigHandler);
        server.runServer ();
    }
    catch ( std::exception& e ) {
        std::cout << "exception" << std::endl;
        std::cout << e.what() << std::endl;
        return (1);
    }
}