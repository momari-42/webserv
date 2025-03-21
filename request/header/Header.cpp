/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:32:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/22 14:51:46 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header ( std::string &errorCode ) : errorCode(errorCode) {
    (void) this->errorCode;
}

void toUpperCase(std::string &str ) {
    // std::cout << "this is the str before : " << str << std::endl;
    for ( size_t n = 0; n < str.size(); n++) {
        str.at(n) = toupper(str.at(n));
        if (str.at(n) == '-')
            str.at(n) = '_';
    }
    // std::cout << "this is the str after : " << str << std::endl;
}

void toLowerCase(std::string &str ) {
    // std::cout << "this is the str before : " << str << std::endl;
    for ( size_t n = 0; n < str.size(); n++) {
        str.at(n) = tolower(str.at(n));
    }
    // std::cout << "this is the str after : " << str << std::endl;
}

bool checkHost(std::string& host) {
    std::vector<std::string> parts;
    std::string word;
    if ( host.find("\t") != std::string::npos )
        return (false);
    while (host.size()) {
        host.erase(0, host.find_first_not_of(" "));
        if (!host.size())
            break;
        word = host.substr(0, host.find_first_of(" "));
        host.erase(0, host.find_first_of(" "));
        parts.push_back(word);
    }
    if ( parts.size() != 1)
        return (false);
    return (true);
}

void Header::setHeader( std::string &header, int& trackingRequestNumber ) {
    std::string portion;
    std::string firstPortion;
    std::string secondPortion;

    this->rest += header;
    if (this->rest.find("\r\n\r\n") != std::string::npos) {
        header = this->rest.substr(this->rest.find("\r\n\r\n") + 4);
        this->rest.erase(this->rest.find("\r\n\r\n") + 2);
        trackingRequestNumber++;
    }
    else {
        header = "";
        return ;
    }
    while (this->rest.find("\r\n") != std::string::npos) {
        if (this->rest.find(":") != std::string::npos && this->rest.find(":") < this->rest.find("\r\n")) {
            portion = this->rest.substr(0, this->rest.find("\r\n"));
            firstPortion = portion.substr(0, portion.find(":"));
            if (firstPortion.find(" ") != std::string::npos || firstPortion.find("\t") != std::string::npos) {
                // std::cout << "from setHeader" << std::endl;
                this->errorCode = "400";
                return ;
            }
            toUpperCase(firstPortion);
            portion.erase(0, portion.find(":") + 1);
            if (portion.find_first_not_of(" ") != std::string::npos)
                secondPortion = portion.substr(portion.find_first_not_of(" "));
            else
                secondPortion = portion.substr(0);
            if ( firstPortion == "HOST" && (!checkHost(secondPortion) || this->httpHeadersMap.count("HOST"))) {
                this->errorCode = "400";
                return;
            }
            toLowerCase(secondPortion);
            this->httpHeadersMap[firstPortion] = secondPortion;
            this->rest.erase(0, this->rest.find("\r\n") + 2);
        }
        else {
            // std::cout << "from setHeader" << std::endl;
            this->errorCode = "400";
            return ;
        }
    }
    if (!this->httpHeadersMap.count("HOST"))
        this->errorCode = "400";
}


std::string Header::getValue(const std::string& key) {
    return this->httpHeadersMap[key];
}
// void Header::print() {
//     for (std::map<std::string, std::string>::iterator it = this->httpHeadersMap.begin(); it != this->httpHeadersMap.end(); it++) {
//         std::cout << ">" << (*it).first << "<->" << (*it).second << "<" << std::endl;
//     }
// }

Header::~Header ( void ) {
    
}

void Header::resetAttributes (void) {
    std::map<std::string, std::string> tmp;

    this->rest = "";
    this->httpHeadersMap  = tmp;
}

std::map<std::string, std::string> &Header::getHttpHeadersMap( void ) {
    return (this->httpHeadersMap);
}