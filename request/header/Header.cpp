/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:32:10 by zaelarb           #+#    #+#             */
/*   Updated: 2025/01/26 12:54:05 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

Header::Header ( void ) {
}

void Header::setHeader( const std::string &header ) {
    std::string portion;
    std::string firstPortion;
    std::string secondPortion;
    std::string request = this->rest + header;

    // std::cout << "HHHHh : " << request.find(":", 0, 21) << std::endl;
    while (request.find("\r\n") != std::string::npos) {
        if (request.find(":") < request.find("\r\n")) {
            portion = request.substr(0, request.find("\r\n"));
            firstPortion = portion.substr(0, portion.find(":"));

            if (firstPortion.find(" ") != std::string::npos || firstPortion.find("\t") != std::string::npos) {
                // if the field is HOST header we must genrate a error for any find of /t or space
                // 400 Bad Request
            }
            portion.erase(0, portion.find(":") + 1);
            secondPortion = portion.substr(portion.find_first_not_of(" "));
            this->httpHeadersMap[firstPortion] = secondPortion;
            request.erase(0, request.find("\r\n") + 2);
        }
        else {
            // 400 Bad Request
        }
    }
    this->rest = request;
}


void Header::print() {
    for (std::map<std::string, std::string>::iterator it = this->httpHeadersMap.begin(); it != this->httpHeadersMap.end(); it++) {
        std::cout << "\033[31m" << (*it).first << " <-> " << (*it).second << "\033[0m" << std::endl;
    }
}
// Header::Header ( const std::string& request ) {
//     size_t          number = 0;
//     std::string     tempraryHeader;
//     std::string     portion;
//     std::string     firstPortion;
//     std::string     secondPortion;

//     this->header = request;
//     tempraryHeader = this->header;
//     if (tempraryHeader.find("\r\n\r\n") != std::string::npos) {
//         this->header.erase(0, this->header.find("\r\n\r\n"));
//         tempraryHeader.erase(tempraryHeader.find("\r\n\r\n"), tempraryHeader.size());
//     }
//     number = tempraryHeader.find("\r\n");
//     if (number == std::string::npos)
//         number -= 2;
//     tempraryHeader.erase(0, number + 2);
//     while (tempraryHeader.size()) {
//         number = tempraryHeader.find("\r\n");
//         portion = tempraryHeader.substr(0, number);
//         firstPortion = portion.substr(0, portion.find(":"));
//         secondPortion = portion.substr(portion.find(":") + 1, portion.size());
//         if (secondPortion.find_first_not_of(" ") != std::string::npos)
//             secondPortion.erase(0, secondPortion.find_first_not_of(" ") != std::string::npos);
//         this->httpHeadersMap.insert(std::pair<std::string, std::string>(firstPortion, secondPortion));
//         if (number == std::string::npos)
//             number-=2;
//         tempraryHeader.erase(0, number + 2);
//     }

//     // Get the value of the boundary if i found it

//     std::map<std::string, std::string>::iterator it = this->httpHeadersMap.find("Content-Type");
//     if (it != this->httpHeadersMap.end()) {
//         secondPortion = (*it).second;
//         if (secondPortion.find("boundary")) {
//             this->boundary = secondPortion.substr(secondPortion.find("boundary") + 1);
//         }
//     }
    
//     // for (std::map<std::string, std::string>::iterator it = this->httpHeadersMap.begin(); it != this->httpHeadersMap.end(); it++) {
//     //     std::cout << "\033[31m" << (*it).first << " <-> " << (*it).second << "\033[0m" << std::endl;
//     // }
// }


Header::~Header ( void ) {
    
}


// int main () {
//     std::string http_request = (
//     "GET  HTTP/1.1\r\n"
//     "Host: nginx.org\r\n"
//     "Sec-Ch-Ua: \r\n"
//     "Sec-Ch-Ua-Mobile: ?0\r\n"
//     "Sec-Ch-Ua-Platform: \"\"\r\n"
//     "Upgrade-Insecure-Requests: 1\r\n"
//     "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.5735.134 Safari/537.36\r\n"
//     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
//     "Sec-Fetch-Site: none\r\n"
//     "Sec-Fetch-Mode: navigate\r\n"
//     "Sec-Fetch-User: ?1\r\n"
//     "Sec-Fetch-Dest: document\r\n"
//     "Accept-Encoding: gzip, deflate\r\n"
//     "Accept-Language: en-US,en;q=0.9\r\n"
//     "Connection: close\r\n"
//     "\r\n"
//     );
//     // strtok((char *)http_request.c_str(), "\n");
//     // strtok((char *)http_request.c_str(), "\r\n");
//     // std::cout << http_request << std::endl;
//     Header header(http_request);
// }