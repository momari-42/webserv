/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:21:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/23 17:05:13 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "../socket/Socket.hpp"
#include "Location.hpp"
#include "../socket/Socket.hpp"



class Location;
class Socket;

class ConfigFile
{
private:
    std::vector <std::string> serverName;
    std::map<size_t, std::string> ports;
    std::map<std::string, std::string> errorPages;
    std::vector<std::string> indexs;
    std::vector<Socket> sockets;
    std::string root;
    std::map<std::string, Location> locations;
    size_t bodyLimit;
    size_t URILimit;
    
public:
    ConfigFile();

    void parseServerInfo(std::string& serverInfo);
    void setLocations(std::string &serverInfo);
    void setPorts(std::vector<std::string> args);
    void setServerNames(std::vector<std::string> args);
    void setErrorPages(std::vector<std::string> args);
    void setBodyLimit(std::vector<std::string> args);
    void setURILimit(std::vector<std::string> args);
    void setIndex(std::vector<std::string> args);
    void setRoot(std::vector<std::string> args);
    std::string &getRoot( );

    std::map<size_t, std::string> getPorts();
    size_t getURILimit();
    size_t getBodyLimit();
    std::map<std::string, Location>& getLocations();
    
    // Run server functions
    // void runServer();

    // Additional function to remove later
    void showServerConfig();
    ~ConfigFile();
};