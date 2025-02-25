/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:20:39 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/23 10:57:05 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {
    this->listing = false;
}

void Location::setIndex(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() < 1)
        throw ErrorHandling("Wrong number of arguments in index");
    else {
        for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
            this->indexs.push_back(*it);
    }
}

void Location::setRoot(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 1)
        throw ErrorHandling("Wrong number of arguments in Root");
    else
        this->root = args[0];
}

bool checkMethode(std::string& methode) {
    if (methode.compare("GET") && methode.compare("POST") && methode.compare("DELETE"))
        return false;
    return true;
}

void Location::setMethods(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() < 1)
        throw ErrorHandling("Wrong number of arguments in allowed methods");
    else {
        for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
            if (checkMethode(*it))   
                this->methods.push_back(*it);
            else
                throw ErrorHandling("Wrong Methode");
        }
    }
}
void Location::setListing(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 1)
        throw ErrorHandling("Wrong number of arguments in listing");
    else if(args[0].compare("on") && args[0].compare("off"))
        throw ErrorHandling("Wrong value of listing argument");
    else if (args[0].compare("on") == 0)
        this->listing = true;
    else
        this->listing = false;
}
void Location::setDirection(std::vector<std::string> args) {
    args.erase(args.begin());
    if (args.size() != 2)
        throw ErrorHandling("Wrong number of arguments in return");
    else if(!ft_isdigits(args[0]))
        throw ErrorHandling("return code is not digit");
    else 
        this->redirection[args[0]] = args[1];
}

void Location::parseLocationInfo(std::string& locationInfo) {
    int index;
    std::string tmp;
    std::vector<std::string> line;
    while (locationInfo.length() > 0) {
        index = 0;
        while (locationInfo[index] == ' ' || locationInfo[index] == '\n')
            index++;
        locationInfo.erase(0, index);
        if (locationInfo.length() == 0)
            break;
        if (locationInfo.find(';') != std::string::npos) {
            tmp = locationInfo.substr(0, locationInfo.find(';') + 1);
            locationInfo.erase(0, locationInfo.find(';') + 1);
        } else
            throw ErrorHandling("Line not closed with semicolone ;");
        if (tmp.find('\n') != std::string::npos)
            throw ErrorHandling("Line not closed with semicolone ;");
        line = ft_split(tmp);
        if (line.front() == "index")
            setIndex(line);
        else if (line.front() == "root")
            setRoot(line);
        else if (line.front() == "allowed_methods")
            setMethods(line);
        else if (line.front() == "listing")
            setListing(line);
        else if (line.front() == "return")
            setDirection(line);
        else
            throw ErrorHandling("Element not found");
    }
}

void Location::showLocation() {
    std::cout << "\tRoot : |" << this->root << "|" << std::endl;
    std::cout << "\tIndexes : ";
    for (std::vector<std::string>::iterator it = this->indexs.begin(); it != this->indexs.end(); it++)
        std::cout << "|" << *it << "|";
    std::cout << std::endl;
    std::cout << "\tMethodes : ";
    for (std::vector<std::string>::iterator it = this->methods.begin(); it != this->methods.end(); it++)
        std::cout << "|" << *it << "|";
    std::cout << std::endl;
    std::cout << "\tListning : " << this->listing << std::endl;
    std::cout << "\tRedirection : ";
    for (std::map<std::string, std::string>::iterator it = this->redirection.begin(); it != this->redirection.end(); it++)
        std::cout << "|" << (*it).first << "|" << (*it).second << "|" << std::endl;
    std::cout << std::endl;
}

std::vector<std::string>    &Location::getMethods() {
    return (this->methods);
}

std::map<std::string, std::string>      &Location::getRedirection() {
    return (this->redirection);
}

std::vector<std::string>  &Location::getIndexs() {
    return (this->indexs);
}

std::string Location::getRoot() {
    return this->root;
}

Location::~Location() {
    
}