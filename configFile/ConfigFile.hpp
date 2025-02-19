/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:21:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/19 15:18:03 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/sources.hpp"

class Location;

class ConfigFile
{
private:
    std::vector <std::string> serverName;
    std::map<int, std::string> ports;
    std::map<std::string, std::string> errorPages;
    std::vector<std::string> indexs;
    std::vector<Socket> sockets;
    std::string root;
    std::map<std::string, Location> locations;
    int bodyLimit;
    
public:
    ConfigFile();

    // Parssing functions
    void parseServerInfo(std::string& serverInfo);
    void setLocations(std::string &serverInfo);
    void setPorts(std::vector<std::string> args);
    void setServerNames(std::vector<std::string> args);
    void setErrorPages(std::vector<std::string> args);
    void setBodyLimit(std::vector<std::string> args);
    void setIndex(std::vector<std::string> args);
    void setRoot(std::vector<std::string> args);

    std::map<int, std::string> getPorts() const;
    // Run server functions
    // void runServer();

    // Additional function to remove later
    void showServerConfig();
    ~ConfigFile();
};