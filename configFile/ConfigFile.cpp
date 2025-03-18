/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:03:12 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/16 22:46:35 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ServerConfig::ServerConfig() {
    this->URILimit = 4096;
    this->bodyLimit = 2147483647;
}

bool ServerConfig::operator==(ServerConfig& obj) {
    for (std::vector<std::pair<const std::string, const std::string> >::iterator it = this->ports.begin(); it != this->ports.end(); it++) {
        for (std::vector<std::pair<const std::string, const std::string> >::iterator iter = obj.ports.begin(); iter != obj.ports.end(); iter++) {
            if (it->first == iter->first && it->second == iter->second) {
                for (std::vector<std::string>::iterator name = this->names.begin(); name != this->names.end(); name++) {
                    for (std::vector<std::string>::iterator name1 = obj.names.begin(); name1 != obj.names.end(); name1++) {
                        // std::cout << *name << " " << *name1 << std::endl;
                        if (*name == *name1)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ServerConfig::isExistName(const std::string& name) {
    for (std::vector<std::string>::iterator it = this->names.begin(); it != this->names.end(); ++it) {
        if (name == (*it))
            return true;
    }
    return false;
}
void ServerConfig::showServerConfig() {
    std::cout <<  "------------------------------ Start Server ------------------------" << std::endl;
    std::cout << "Server Names : ";
    for (std::vector<std::string>::iterator it = this->names.begin(); it != this->names.end(); ++it) {
        std::cout << "|" << (*it) << "|";
    }
    std::cout << std::endl <<"Server Ports : " << std::endl;
    for (std::vector<std::pair<const std::string, const std::string> >::iterator it = this->ports.begin(); it != this->ports.end(); it++)
        std::cout << "|" << it->first << "|" << it->second << "|" << std::endl;
    std::cout <<"Error Pages : \n";
    for (std::map<std::string, std::string>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); it++)
        std::cout << "|" << (*it).first << "|" << (*it).second << "|" << std::endl;
    std::cout << "Server Indexs : ";
    for (std::vector<std::string>::iterator it = this->index.begin(); it != this->index.end(); it++) {
        std::cout << "|" << (*it) << "|";
    }
    std::cout << "\nRoot : " << "|" << this->root << "|" << std::endl;
    std::cout << "Body Limit : " << this->bodyLimit << std::endl;
    std::cout << "Locations"<< std::endl;
    for (std::map<std::string, Location>::iterator it = this->locations.begin(); it != this->locations.end(); it++) {
        std::cout << (*it).first << std::endl;
        // (*it).second.showLocation();
    }
    // std::cout <<  "------------------------------ End Server ------------------------" << std::endl;
}

void Location::showLocation() {
    std::cout << "\tRoot : |" << this->root << "|" << std::endl;
    std::cout << "\tIndexes : ";
    for (std::vector<std::string>::iterator it = this->index.begin(); it != this->index.end(); it++)
        std::cout << "|" << *it << "|";
    std::cout << std::endl;
    std::cout << "\tMethodes : ";
    for (std::vector<std::string>::iterator it = this->methods.begin(); it != this->methods.end(); it++)
        std::cout << "|" << *it << "|";
    std::cout << std::endl;
    std::cout << "\tListning : " << this->listing << std::endl;
    std::cout << "\tRedirection : ";
    std::cout << redirection.first << " " << redirection.second << std::endl;
    // for (std::map<std::string, std::string>::iterator it = this->redirection.begin(); it != this->redirection.end(); it++)
    //     std::cout << "|" << (*it).first << "|" << (*it).second << "|" << std::endl;
    std::cout << std::endl;
    // std::cout << "the cgi is : " << cgi[0].first << cgi[0].second << std::endl;
}

void ServerConfig::setPorts(std::vector<std::string>& parts) {
    // 0 - 65535
    std::string host, port;
    if (parts.size() != 2)
        throw (ErrorHandling("Wrong Number of argument in listen"));
    else {
        if (parts[1].find(':') != std::string::npos) {
            host = parts[1].substr(0, parts[1].find(':'));
            port = parts[1].substr(parts[1].find(':') + 1);
        } else {
            if (ft_isdigits(parts[1])) {
                host = "127.0.0.1";
                port = parts[1];
            } else {
                host = parts[1];
                port = "8080";
            }
        }
        if(!ft_isdigits(port) || port.size() > 5)
            throw ErrorHandling("listen port is not digit or too long");
        else
            this->ports.push_back(std::make_pair(port, host));
        if (atoi(port.c_str()) < 0 || atoi(port.c_str()) > 65535)
            throw ErrorHandling("One of the ports is out of range");
    } 
}

void ServerConfig::setRoot(std::vector<std::string>& parts) {
    if (parts.size() != 2)
        throw (ErrorHandling("Wrong Number of argument in root"));
    else
        this->root = parts[1];
}

void ServerConfig::setNames(std::vector<std::string>& parts) {
    if (parts.size() < 2)
        throw ErrorHandling("Wrong number of arguments in server_name");
    else {
        for (std::vector<std::string>::iterator it = parts.begin() + 1; it != parts.end(); it++)
            this->names.push_back(*it);
    }
}

void ServerConfig::setIndex(std::vector<std::string>& parts) {
    if (parts.size() < 2)
        throw ErrorHandling("Wrong number of arguments in index");
    else {
        for (std::vector<std::string>::iterator it = parts.begin() + 1; it != parts.end(); it++)
            this->index.push_back(*it);
    }
}

void ServerConfig::setErrorPage(std::vector<std::string>& parts) {
    if (parts.size() != 3)
        throw ErrorHandling("Wrong number of arguments in Error page");
    else if(!ft_isdigits(parts[1]) || parts[1].size() != 3 \
            || atoi(parts[1].c_str()) > 599 || atoi(parts[1].c_str()) < 400)
        throw ErrorHandling("Error page code is invalid");
    else 
        this->errorPages[parts[1]] = parts[2];
}

void ServerConfig::setBodyLimit(std::vector<std::string>& parts) {
    if (parts.size() != 2)
        throw ErrorHandling("Wrong number of arguments in Body Limit");
    else if(!ft_isdigits(parts[1]))
        throw ErrorHandling("Body limit is not digit");
    else if (parts[1].length() > 10)
        throw ErrorHandling("Invalid argument in body limit");
    else
        this->bodyLimit = atol(parts[1].c_str());
    if (this->bodyLimit == 0 && parts[1].compare("0"))
        throw ErrorHandling("Wrong Number in body limit");
    else if (this->bodyLimit > LONG_MAX)
        throw ErrorHandling("Number in body limit to large");
}

void ServerConfig::setURILimit(std::vector<std::string>& parts) {
    if (parts.size() != 2)
        throw ErrorHandling("Wrong number of arguments in URI Limit");
    else if(!ft_isdigits(parts[1]))
        throw ErrorHandling("URI limit is not digit");
    else if (parts[1].length() > 7 || atoi(parts[1].c_str()) <= 0)
        throw ErrorHandling("Invalid argument in uri limit");
    else
        this->URILimit = atol(parts[1].c_str());
    if (this->URILimit == 0 && parts[1].compare("0"))
        throw ErrorHandling("Wrong Number in body limit");
    else if (this->URILimit > 10000)
        throw ErrorHandling("Number in URI limit to large");
}

void ServerConfig::setLocation(std::string &serverInfo) {
    std::string first = serverInfo.substr(0, serverInfo.find('{') + 1);
    std::vector<std::string> parts;
    // std::cout << first << std::endl;
    ft_split(first, parts);
    // std::cout << parts[0] << " " << parts[1] << " " << parts[2] << std::endl;
    if (parts.size() != 3)
        throw ErrorHandling("Syntax Error in location args");
    // else if (parts[1][parts[1].size() - 1] != '/')
    //     throw ErrorHandling("Location path should be ended with [/]");
    std::string location = serverInfo.substr(0, serverInfo.find('}') + 1);
    serverInfo.erase(0, serverInfo.find('}') + 2);
    // location.erase(0, location.find('{') + 1);
    location = location.substr(0, location.find('}') + 1);
    Location loc;
    loc.parseLocation(location);
    this->locations[parts[1]] = loc;
}

void ServerConfig::parse(std::string& config) {
    std::string line;
    std::vector<std::string> parts;
    while (config.size()) {
        parts.clear();
        line = config.substr(0, config.find('\n'));
        line.erase(0, line.find_first_not_of(" \t\n"));
        if (!line.size()) {
            config.erase(0, config.find('\n') + 1);
            continue;
        }
        if (line.compare(0, 9, "location ") && line.find(';') == std::string::npos)
            throw (ErrorHandling("Syntax Error: Line not closed with [;]"));
        ft_split(line, parts);
        if (parts[0] == "listen") // Done
            setPorts(parts);
        else if (parts[0] == "server_name") // Done
            setNames(parts);
        else if (parts[0] == "index") // Done
            setIndex(parts);
        else if (parts[0] == "root") // Done
            setRoot(parts);
        else if (parts[0] == "error_page") // Done
            setErrorPage(parts);
        else if (parts[0] == "body_limit") // Done
            setBodyLimit(parts);
        else if (parts[0] == "uri_limit") // Done
            setURILimit(parts);
        else if (parts[0] == "location") { // Done
            std::string location = config.substr(0, config.find('}') + 2);
            setLocation(location);
            config.erase(0, config.find('}') + 2);
            continue;
        } else
            throw (ErrorHandling("Syntax Error: Unknown Element"));
        config.erase(0, config.find('\n') + 1);
    }
    checkRequirement();
}

void ServerConfig::checkRequirement() {
    if (!this->ports.size())
        this->ports.push_back(std::make_pair("8080", "127.0.0.1"));
    if (!this->names.size())
        this->names.push_back("localhost");
    if (!this->index.size())
        throw (ErrorHandling("You need at least a default Index"));
    if (!this->root.size())
        throw(ErrorHandling("You need a default Root"));
}

void Location::setPath(std::string& path) {
    this->path = path;
}

void Location::setRoot(std::vector<std::string>& parts) {
    if (parts.size() != 2)
        throw (ErrorHandling("Wrong Number of argument in root in Location " + this->path));
    else
        this->root = parts[1];
}

void Location::setIndex(std::vector<std::string>& parts) {
    if (parts.size() < 2)
        throw ErrorHandling("Wrong number of arguments in index of Location " + this->path);
    else {
        for (std::vector<std::string>::iterator it = parts.begin() + 1; it != parts.end(); it++)
            this->index.push_back(*it);
    }
}

void Location::setMethods(std::vector<std::string>& parts) {
    if (parts.size() < 2)
        throw ErrorHandling("Wrong number of arguments in methods of Location " + this->path);
    else {
        for (std::vector<std::string>::iterator it = parts.begin() + 1; it != parts.end(); it++) {
            if (!(*it).compare("GET") || !(*it).compare("POST") || !(*it).compare("DELETE"))
                this->methods.push_back(*it);
            else
                throw ErrorHandling("Invalid method in Location " + this->path);
        }
    }
}

void Location::setListing(std::vector<std::string>& parts) {
    if (parts.size() != 2)
        throw ErrorHandling("Wrong number of arguments in listing of Location " + this->path);
    else {
        if (parts[1].compare("off") && parts[1].compare("on"))
            throw ErrorHandling("Invalid argument in listing of Location " + this->path);
        else if (!parts[1].compare("on"))
            this->listing = true;
        else
            this->listing = false;
    }
}

void Location::setRedirection(std::vector<std::string>& parts) {
    if (parts.size() != 3)
        throw ErrorHandling("Wrong number of arguments in return of Location " + this->path);
    else if(!ft_isdigits(parts[1]) || parts[1].size() != 3 \
            || atoi(parts[1].c_str()) > 399 || atoi(parts[1].c_str()) < 300)
        throw ErrorHandling("Return code is invalid");
    else 
        this->redirection = std::make_pair(parts[1], parts[2]);
}

void Location::setCgi(std::vector<std::string>& parts) {
    if (parts.size() != 3)
        throw ErrorHandling("Wrong number of arguments in cgi of Location " + this->path);
    if (parts[1][0] != '.' || parts[1].size() < 2)
        throw ErrorHandling("Cgi Extention invalid of Location " + this->path);
    else
        this->cgi[parts[1]] = parts[2];
}

void Location::parseLocation(std::string& config) {
    std::vector<std::string> parts;
    std::string line;
    line = config.substr(0, config.find('\n') + 1);
    ft_split(line, parts);
    if (parts.size() != 3 || parts[2].compare("{"))
        throw ErrorHandling("Wrong argument in location");
    else
        this->path = parts[1];
    config.erase(0, config.find('\n') + 1);
    while (config.size()) {
        parts.clear();
        if (!config.compare("}")) {
            config.erase(0);
            break;
        }
        line = config.substr(0, config.find('\n') + 1);
        line.erase(0, line.find_first_not_of(" \t\n"));
        if (!line.size()) {
            config.erase(0, config.find('\n') + 1);
            continue;
        }
        ft_split(line, parts);
        if (parts[0] == "index")
            setIndex(parts);
        else if (parts[0] == "root")
            setRoot(parts);
        else if (parts[0] == "allowed_methods")
            setMethods(parts);
        else if (parts[0] == "return")
            setRedirection(parts);
        else if (parts[0] == "cgi")
            setCgi(parts);
        else if (parts[0] == "listing")
            setListing(parts);
        else
            throw (ErrorHandling("Syntax Error: Unknown Element in Location " + this->path));
        config.erase(0, config.find('\n') + 1);
    }
}

// ============================ Geters ================================

std::vector<std::pair<const std::string, const std::string> > ServerConfig::getPorts() {
    return ports;
}

std::string ServerConfig::getRoot( std::string& path, std::string &errorCode) {
    std::vector<std::string>        matchedLocations;
    std::string                     bestMatchedLocation;

    this->matchedLocation = "";
    for (std::map<std::string, Location>::iterator it = this->locations.begin(); it != this->locations.end(); it++) {
        if ( path.find(it->first) == 0 )
            matchedLocations.push_back(it->first);
    }
    for (std::vector<std::string>::iterator it = matchedLocations.begin(); it != matchedLocations.end(); it++) {
        if ( (*it).size() > bestMatchedLocation.size()) {
            bestMatchedLocation = *it;
        }
    }
    if (bestMatchedLocation.size()) {
        this->matchedLocation = bestMatchedLocation;
        path.erase(0, bestMatchedLocation.size());
        if (this->locations[bestMatchedLocation].root.size())
            return this->locations[bestMatchedLocation].root;
        else
            return this->root;
    }
    else
        errorCode = "404";
    return "";
}

std::vector<std::string> &ServerConfig::getIndex() {
    std::map<std::string, Location>::iterator it = this->locations.find(this->matchedLocation);
    if (it != this->locations.end() && it->second.index.size()) {
        return this->locations[this->matchedLocation].index;
    }
    return (this->index);
}

size_t  ServerConfig::getBodyLimit() {
    return (this->bodyLimit);
}

size_t      ServerConfig::getURILimit() {
    return (this->URILimit);
}

std::string &ServerConfig::getMatchedLocation() {
    return (this->matchedLocation);
}

std::map<std::string, Location> &ServerConfig::getLocations() {
    return (this->locations);
}