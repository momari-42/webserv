/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:00:21 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/13 15:54:31 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "../errorHandling/ErrorHandling.hpp"
#include "../utils/utils.hpp"

class Location {
public:
    std::string                                      path;
    std::vector<std::string>                         index;
    std::string                                      root;
    std::vector<std::string>                         methods;
    std::pair<std::string, std::string>              redirection;
    std::vector<std::string>                         cgi;
    bool                                             listing;
    bool                                             upload;

    void parseLocation(std::string& config);
    void showLocation();
private:
    void setPath(std::string& config);
    void setIndex(std::vector<std::string>& parts);
    void setRoot(std::vector<std::string>& parts);
    void setMethods(std::vector<std::string>& parts);
    void setListing(std::vector<std::string>& parts);
    void setRedirection(std::vector<std::string>& parts);
    void setCgi(std::vector<std::string>& parts);

    // Geters
};

class ServerConfig {
private:
    std::vector<std::pair<int, const std::string> > ports;
    std::vector<std::string>                        names;
    std::vector<std::string>                        index;
    std::string                                     root;
    std::map<std::string, Location>                 locations;
    std::map<std::string, std::string>              errorPages;
    size_t                                          bodyLimit;
    size_t                                          URILimit;

    // this for the return of indexes
    std::string                                     matchedLocation;
public:
    ServerConfig();
    void parse(std::string &config);
    void checkRequirement();
    void showServerConfig();
    bool operator==(ServerConfig& obj);

    // Seters
    void        setPorts(std::vector<std::string>& parts);
    void        setRoot(std::vector<std::string>& parts);
    void        setIndex(std::vector<std::string>& parts);
    void        setBodyLimit(std::vector<std::string>& parts);
    void        setURILimit(std::vector<std::string>& parts);
    void        setNames(std::vector<std::string>& parts);
    void        setErrorPage(std::vector<std::string>& parts);
    void        setLocation(std::string& config);

    bool        isExistName(const std::string& name);
    // Geters

    
    std::vector<std::pair<int, const std::string> > getPorts();
    std::string getRoot( std::string& path, std::string &errorCode);
    std::vector<std::string> &getIndex();
    // std::string getRedirection(const std::string& path);
    size_t      getBodyLimit();
    size_t      getURILimit();
    std::string &getMatchedLocation();
    std::map<std::string, Location> &getLocations();
    // bool        getListing(const std::string& path);
    // bool        isAllowedMethod(const std::string& path, const std::string& method);
};