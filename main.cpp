/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:41:11 by momari            #+#    #+#             */
/*   Updated: 2025/03/14 11:45:12 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "errorHandling/ErrorHandling.hpp"
#include <fstream>
#include <iostream>

void resolveLine(std::string &line) {
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '\t')
            line[i] = ' ';
        if ((line[i] == '{' || line[i] == ';') && line[i + 1] != '\n')
            line.insert(i + 1, "\n");
        if (line[i + 1] == '}' && line[i] != '\n')
            line.insert(i + 1, "\n");
        if (line[i + 1] == '{' && line[i] != ' ')
            line.insert(i + 1, " ");
    }
}

bool areClosed(std::string& file) {
    int closed = 0;
    for (size_t i = 0; i < file.size(); i++)
    {
        if (file[i] == '{')
            closed++;
        else if (file[i] == '}')
            closed--;
    }
    if (!closed)
        return true;
    return false;
    
}

void readConfigFile(std::fstream& configFile, std::string& file) {
    std::string line;
    while (getline(configFile, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.find('#') != std::string::npos)
            line.erase(line.find('#'));
        if (line.size() == 0)
            continue;
        line += '\n';
        resolveLine(line);
        line.erase(line.find('\n'), line.find_first_not_of(" \t"));
        file.append(line);
    }
    if (!areClosed(file))
        throw ErrorHandling("Syntax Error");
}

int main(int ac, char **av) {

    if (ac != 2) {
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        std::cerr << "Program work with: ./webserv [configuration file]" << std::endl;
        return 1;
    }
    try {
        std::fstream configFile(av[1]);
        if (!configFile.is_open()) {
            throw ErrorHandling("Config File Can't Open!!!!");
        }
        std::string config;
        readConfigFile(configFile, config);
        Server server(config);
        server.startServer();

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}