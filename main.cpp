/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:41:11 by momari            #+#    #+#             */
/*   Updated: 2025/02/19 21:21:49 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "include/sources.hpp"
#include "server/Server.hpp"
#include "configFile/ConfigFile.hpp"

int main(int ac, char **av) {

    if (ac != 2) {
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        return 1;
    }
    try {
        (void)av;
        std::vector<ConfigFile> config;
        parseConfigFile(config, av[1]);
        Server server(config);
        server.startServer();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    // signal(SIGPIPE, SIG_IGN);

    // try {
    //     std::vector<int> vec;
    //     vec.push_back(9999);
    //     Server server(vec);
    //     server.startServer();
    // }
    // catch ( std::exception& e ) {
    //     std::cout << "exception" << std::endl;
    //     std::cout << e.what() << std::endl;
    //     return (1);
    // }
}