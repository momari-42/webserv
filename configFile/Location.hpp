/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:21:02 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/23 10:56:23 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../include/sources.hpp"

class Location
{
private:
    // std::string location;
    std::string                         root;
    std::vector<std::string>            methods;
    std::vector<std::string>            indexs;
    bool                                listing;
    std::map<std::string, std::string>  redirection;
public:
    Location();
    // void setLocation(std::string& locationInfo);
    void parseLocationInfo(std::string &locationInfo);
    void setIndex(std::vector<std::string> args);
    void setRoot(std::vector<std::string> args);
    void setMethods(std::vector<std::string> args);
    void setListing(std::vector<std::string> args);
    void setDirection(std::vector<std::string> args);
    void showLocation();


    // geters
    std::vector<std::string>                &getMethods();
    std::map<std::string, std::string>      &getRedirection();
    std::string getRoot();
    std::vector<std::string>  &getIndexs();
    ~Location();
};