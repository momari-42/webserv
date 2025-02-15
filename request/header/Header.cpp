/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:32:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/12 16:24:10 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header ( std::string &errorCode ) : errorCode(errorCode) {
    (void) this->errorCode;
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
                this->errorCode = "400";
                return ;
            }
            portion.erase(0, portion.find(":") + 1);
            if (portion.find_first_not_of(" ") != std::string::npos)
                secondPortion = portion.substr(portion.find_first_not_of(" "));
            else
                secondPortion = portion.substr(0);
            this->httpHeadersMap[firstPortion] = secondPortion;
            this->rest.erase(0, this->rest.find("\r\n") + 2);
        }
        else {
            this->errorCode = "400";
            return ;
        }
    }
    if (trackingRequestNumber == 2 && this->rest.size()) {
        // 400 Bad Request
    }
}


std::string Header::getValue(const std::string& key) {
    return this->httpHeadersMap[key];
}
void Header::print() {
    for (std::map<std::string, std::string>::iterator it = this->httpHeadersMap.begin(); it != this->httpHeadersMap.end(); it++) {
        std::cout << ">" << (*it).first << "<->" << (*it).second << "<" << std::endl;
    }
}

Header::~Header ( void ) {
    
}

void Header::resetAttributes (void) {
    std::map<std::string, std::string> tmp;

    this->rest = "";
    this->httpHeadersMap  = tmp;
}