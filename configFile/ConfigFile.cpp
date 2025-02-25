/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:22:20 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/22 16:38:45 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::ConfigFile(){
    this->URILimit = 4096;
    std::cout << "-------------Default---------------" << std::endl;
    std::cout << this->URILimit << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void ConfigFile::setPorts(std::vector<std::string> args) {
    std::string port;
    std::string host;
    args.erase(args.begin());
    if (args.size() != 1)
        throw ErrorHandling("Wrong number of arguments in listen");
    else {
        if (args[0].find(':') != std::string::npos) {
            host = args[0].substr(0, args[0].find(':'));
            port = args[0].substr(args[0].find(':') + 1);
        } else {
            host = "127.0.0.1";
            port = args[0];
        }
        if(!ft_isdigits(port))
            throw ErrorHandling("listen port is not digit");
        else
            this->ports[atoi(port.c_str())] = host;
    }
}

void ConfigFile::setServerNames(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() < 1)
        throw ErrorHandling("Wrong number of arguments in server name");
    else {
        for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
            this->serverName.push_back(*it);
    }
}

void ConfigFile::setIndex(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() < 1)
        throw ErrorHandling("Wrong number of arguments in index");
    else {
        for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
            this->indexs.push_back(*it);
    }
}

void ConfigFile::setRoot(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 1)
        throw ErrorHandling("Wrong number of arguments in Root");
    else
        this->root = args[0];
}

void ConfigFile::setErrorPages(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 2)
        throw ErrorHandling("Wrong number of arguments in Error page");
    else if(!ft_isdigits(args[0]))
        throw ErrorHandling("Error page code is not digit");
    else 
        this->errorPages[args[0]] = args[1];
}

void ConfigFile::setBodyLimit(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 1)
        throw ErrorHandling("Wrong number of arguments in Body Limit");
    else if(!ft_isdigits(args[0]))
        throw ErrorHandling("Body limit is not digit");
    else if (args[0].length() > 7 || atoi(args[0].c_str()) <= 0)
        throw ErrorHandling("Invalid number in body limit");
    else
        this->bodyLimit = atoi(args[0].c_str());
}

void ConfigFile::setURILimit(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 1)
        throw ErrorHandling("Wrong number of arguments in URL Limit");
    else if(!ft_isdigits(args[0]))
        throw ErrorHandling("URL limit is not digit");
    else
        this->URILimit = atoi(args[0].c_str());
    std::cout << "-------------Config---------------" << std::endl;
    std::cout << this->URILimit << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void ConfigFile::setLocations(std::string &serverInfo) {
    std::string first = serverInfo.substr(0, serverInfo.find('{') + 1);
    std::vector<std::string> parts = ft_split(first);
    if (parts.size() != 3)
        throw ErrorHandling("Syntax Error in location args");
    std::string location = serverInfo.substr(0, serverInfo.find('}') + 1);
    serverInfo.erase(0, serverInfo.find('}') + 1);
    location.erase(0, location.find('{') + 1);
    location = location.substr(0, location.find('}'));
    Location loc;
    loc.parseLocationInfo(location);
    // if (parts[1].find_last_of('/') == parts[1].length() - 1)
    //     parts[1].erase(parts[1].find_last_of('/'));
    this->locations[parts[1]] = loc;
}


void ConfigFile::parseServerInfo(std::string& serverInfo) {
    int index;
    std::string tmp;
    std::vector<std::string> line;
    while (serverInfo.length() > 0) {
        index = 0;
        while (serverInfo[index] == ' ' || serverInfo[index] == '\n')
            index++;
        serverInfo.erase(0, index);
        if (serverInfo.length() == 0)
            break;
        if (serverInfo.compare(0, 9, "location ") == 0)
            setLocations(serverInfo);
        else {
            if (serverInfo.find(';') != std::string::npos) {
                tmp = serverInfo.substr(0, serverInfo.find(';') + 1);
                serverInfo.erase(0, serverInfo.find(';') + 1);
            } else
                throw ErrorHandling("Line not closed with semicolone ;");
            if (tmp.find('\n') != std::string::npos)
                throw ErrorHandling("Line not closed with semicolone ;");
            line = ft_split(tmp);
            if (line.front() == "listen")
                setPorts(line);
            else if (line.front() == "server_name")
                setServerNames(line);
            else if (line.front() == "index")
                setIndex(line);
            else if (line.front() == "root")
                setRoot(line);
            else if (line.front() == "error_page")
                setErrorPages(line);
            else if (line.front() == "body_limit")
                setBodyLimit(line);
            else if (line.front() == "uri_limit")
                setURILimit(line);
            else
                throw ErrorHandling("Element not found");
        }
    }
}

std::map<size_t, std::string> ConfigFile::getPorts() {
    return this->ports;
}

// void ConfigFile::runServer() {
//     for (std::map<int, std::string>::iterator it = this->ports.begin(); it != this->ports.end(); it++) {
//         Socket sock((*it).first, (*it).second);
//         this->sockets.push_back(sock);
//     }
// }

void ConfigFile::showServerConfig() {
    std::cout <<  "------------------------------ Start Server ------------------------" << std::endl;
    std::cout << "Server Names : ";
    for (std::vector<std::string>::iterator it = this->serverName.begin(); it != this->serverName.end(); it++) {
        std::cout << "|" << (*it) << "|";
    }
    std::cout << std::endl <<"Server Ports : ";
    for (std::map<size_t, std::string>::iterator it = this->ports.begin(); it != this->ports.end(); it++)
        std::cout << "|" << (*it).first << "|" << (*it).second << "|" << std::endl;
    std::cout << std::endl <<"Error Pages : \n";
    for (std::map<std::string, std::string>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); it++)
        std::cout << "|" << (*it).first << "|" << (*it).second << "|" << std::endl;
    std::cout << "\nServer Indexs : ";
    for (std::vector<std::string>::iterator it = this->indexs.begin(); it != this->indexs.end(); it++) {
        std::cout << "|" << (*it) << "|";
    }
    std::cout << "\nRoot : " << "|" << this->root << "|" << std::endl;
    std::cout << "Body Limit : " << this->bodyLimit << std::endl;
    std::cout << "Locations" << std::endl;
    for (std::map<std::string, Location>::iterator it = this->locations.begin(); it != this->locations.end(); it++) {
        std::cout << (*it).first << std::endl;
        (*it).second.showLocation();
    }
    std::cout <<  "------------------------------ End Server ------------------------" << std::endl;
}


size_t ConfigFile::getURILimit() {
    return (this->URILimit);
}

size_t ConfigFile::getBodyLimit() {
    return (this->bodyLimit);
}

std::map<std::string, Location>& ConfigFile::getLocations() {
    return (this->locations);
}

std::string &ConfigFile::getRoot( ) {
    return (this->root);
}

ConfigFile::~ConfigFile() {

}