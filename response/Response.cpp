/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:06 by momari            #+#    #+#             */
/*   Updated: 2025/03/08 14:35:58 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response ( Request *request ) {
    this->request = request;
    // this->socket = soc;
    // this->configFile = soc->getServerConfig(request->getHeader()->getValue("Host"));
    this->isHeaderSent      = false;
    this->isResponseSent    = false;
    this->httpVersion = "HTTP/1.1";

    this->header["Server"] = "momari-zaelarb";
    this->header["Connection"] = "keep-alive";
    this->header["Transfer-Encoding"] = "chunked";
}

void Response::executeCGI ( size_t fd ) {
    (void)fd;
    std::string requestTarget = this->request->getRequestLine()->getRequestTarget();
    std::string queryString;
    std::string fullPathScript;
    int         fds[2];

    size_t queryPos = requestTarget.find("?");
    if (queryPos != std::string::npos) {
        queryString = "QUERY_STRING=";
        queryString += requestTarget.substr(queryPos + 1);
        requestTarget.erase(queryPos);
    }
    setenv("QUERY_STRING", queryString.c_str(), 1);
    fullPathScript = "/Users/momari/cursus/webserv" + requestTarget;
    char *argv[] = {const_cast<char *>("/usr/bin/php"), const_cast<char *>(fullPathScript.c_str()), NULL};
    char *env[] = { const_cast<char *>(queryString.c_str()), NULL};
    pipe(fds);
    int pid = fork();
    if (pid == 0) {
        // close(fds[0]);
        // dup2(fds[1], 1);
        // close(fds[1]);
        execve(argv[0], argv, env);
    }
    else {
        wait(NULL);
    }
}

void Response::makeResponse ( size_t fd ) {
    // if ()
    if (this->request->getIsCgi())
        executeCGI( fd );
    else if (this->request->getRequestLine()->getMethod() == "GET")
        methodGet( fd );
    else if (this->request->getRequestLine()->getMethod() == "POST")
        methodPost( fd );
    else if (this->request->getRequestLine()->getMethod() == "DELETE")
        methodDelete( fd );
}

std::string convertDecimalToHexaToString ( size_t number ) {
    std::ostringstream  stringNumberOne;

    stringNumberOne << std::hex << number;
    return (stringNumberOne.str());
}

void Response::validateRequestTarget() {
    if (isDirectory(this->request->getRequestTarget())) {
        bool isValidPath = false;
        std::vector<std::string> &index = this->configFile->getIndex();
        std::string tempraryRequestTarget;

        if (this->request->getRequestTarget().size() && this->request->getRequestTarget().at(this->request->getRequestTarget().size() - 1) != '/')
            this->request->getRequestTarget() += '/';
        for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
            tempraryRequestTarget = this->request->getRequestTarget() + *it;
            if (access( tempraryRequestTarget.c_str(), F_OK ) != -1) {
                this->request->getRequestTarget() = tempraryRequestTarget;
                isValidPath = true;
                break;
            }
        }
        if (!isValidPath) {
            this->errorCode = "404";
            return ;
        }
    }
    else {
        if (access( this->request->getRequestTarget().c_str(), F_OK ) == -1) {
            this->errorCode = "404";
            return;
        }   
        if (access( this->request->getRequestTarget().c_str(), R_OK ) == -1) {
            this->errorCode = "401";
            return;
        }
    }
}



void Response::sendRedirectionResponse( size_t fd, Location &location ) {
    std::string response;

    response += this->httpVersion + " " + location.redirection.first + " "  + this->statusCodes[location.redirection.first] + CRLF;
    response += "Location: " + location.redirection.second + CRLF + CRLF;
    if (send(fd, response.c_str(), response.size(), 0) == -1) {
        std::cerr << "Error sending data" << std::endl; 
    }
}

void Response::generateHeader ( int fd, std::string &response) {
    // if (configFile.getReturn()) {
    //     // we should generate a return response and return;
    // }
    Location &location =  this->request->getLocation();
    if (!location.redirection.first.empty() && !location.redirection.second.empty()) {
        sendRedirectionResponse(fd, location);
        this->isHeaderSent = true;
        this->isResponseSent = true;
        return;
    }
    validateRequestTarget();
    if (this->errorCode.size())
        return ;
    if (this->request->getRequestLine()->getRequestTarget().find(".") != std::string::npos ) {
        std::map<std::string, std::string>::iterator it = this->mime.find( \
            this->request->getRequestLine()->getRequestTarget().substr(    \
                this->request->getRequestLine()->getRequestTarget().find_last_of(".")));
        if (it != this->mime.end())
            this->header["Content-Type"] = it->second + "; charset=UTF-8";
    }
    response += this->httpVersion + " 200 " + this->statusCodes["200"] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    if (send(fd, response.c_str(), response.size(), 0) == -1) {
        std::cerr << "Error sending data" << std::endl;
        // Handle send error
    }
    setTargetFile();
    this->isHeaderSent = true;
    response.clear();
}

bool isDirectory(std::string &path) {
    DIR *dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return (true);
    }
    return (false);
}

void Response::setConfigFile(ServerConfig* configFile) {
    this->configFile = configFile;
}

// void Response::validateAccessTarget( int fd, std::string &requestTarget ) {
//     bool                            isValidPath = false;
//     std::string                     response;
//     std::vector<std::string>        matchedLocations;
//     std::string                     bestMatchedLocation;
//     std::map<std::string, Location> &locations = this->configFile->getLocations();
//     std::cout << "this is the request target : " << requestTarget << std::endl;

//     for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++) {
//         if ( requestTarget.find(it->first) == 0 ) {
//             matchedLocations.push_back(it->first);
//         }
//     }
//     for (std::vector<std::string>::iterator it = matchedLocations.begin(); it != matchedLocations.end(); it++) {
//         if ( (*it).size() > bestMatchedLocation.size()) {
//             bestMatchedLocation = *it;
//         }
//     }
//     if (bestMatchedLocation.size()) {
//         if (locations[bestMatchedLocation].getRedirection().size()) {
//     // std::cout << "best matched location : " << bestMatchedLocation << std::endl;
//             std::map<std::string, std::string>::iterator it = locations[bestMatchedLocation].getRedirection().begin();
//             response += this->httpVersion + " " + it->first + " "  + this->statusCodes[it->first] + CRLF;
//             response += "Location: " + it->second + CRLF + CRLF;
//             if (send(fd, response.c_str(), response.size(), 0) == -1) {
//                 std::cerr << "Error sending data" << std::endl; 
//             }
//             this->isHeaderSent = true;
//             this->isResponseSent = true;
//             return;
//         }
//         requestTarget.erase(0, bestMatchedLocation.size());
//         requestTarget = locations[bestMatchedLocation].getRoot() + requestTarget;
//         if (isDirectory(requestTarget)) {
//             std::string tempraryPath;
//             std::vector<std::string> &indexs = locations[bestMatchedLocation].getIndexs();

//             if (requestTarget.find("/") == requestTarget.size() - 1)
//                 requestTarget.erase(requestTarget.size() - 1);
//             for (std::vector<std::string>::iterator it = indexs.begin(); it != indexs.end(); it++) {
//                 if ((*it).find("/") != 0)
//                     (*it).insert(0, "/");
//                 tempraryPath =  requestTarget + (*it);
//                 if (access( tempraryPath.c_str(), F_OK ) != -1) {
//                     requestTarget = tempraryPath;
//                     isValidPath = true;
//                     break;
//                 }
//             }
//             if (!isValidPath) {
//                 this->errorCode = "404" ;
//                 return;
//             }
//             // requestTarget += locations[bestMatchedLocation].getIndexs();
//         }
//     }
//     else {
//         if (configFile.getRoot().find("/") == configFile.getRoot().size() - 1)
//             configFile.getRoot().erase(configFile.getRoot().size() - 1);
//         requestTarget = configFile.getRoot() + requestTarget;
//     }
//     std::cout << "404 : " << requestTarget << std::endl;
//     if (access( requestTarget.c_str(), F_OK ) == -1) {
//         this->errorCode = "404";
//         return;
//     }
//     if (access( requestTarget.c_str(), R_OK ) == -1) {
//         this->errorCode = "401";
//         return;
//     }
// }

void Response::methodGet( size_t fd ) {
    std::string         response;
    char                buffer[BUFFER_SIZE_R];
    std::streamsize     bytesRead;

    // if (this->configFile.getr)
    if (!this->isHeaderSent) {
        generateHeader(fd, response);
    }
    if ( this->isResponseSent || this->errorCode.size())
        return;
    memset(buffer, 0, sizeof(buffer));
    targetFile.read(buffer, BUFFER_SIZE_R);
    bytesRead = targetFile.gcount();

    if (bytesRead > 0) {
        std::string content(buffer, bytesRead);
        std::string hexaNumber = convertDecimalToHexaToString(bytesRead); 
        response += hexaNumber + CRLF;
        response += content + CRLF;
        if (send(fd, response.c_str(), response.size(), 0) == -1) {
            std::cout << "Error sending data" << std::endl;
            // Handle send error
        }
    }
    if (targetFile.eof()) {
        response = "0" + std::string(CRLF) + CRLF;
        send(fd, response.c_str(), response.size(), 0);
        this->isResponseSent = true;
        targetFile.close(); // Close the file
    }
}


std::string calculateBodyLength( std::string &body ) {
    std::ostringstream size;

    size << body.size();
    return (size.str());
}

void Response::methodPost( size_t fd ) {
    std::string response;
    std::string body = "File uploaded succefully !!";

    if (this->request->getHeader()->getValue("Connection") == "close") {
        this->header["Connection"] = "close";
    }
    this->header["Content-Length"] = calculateBodyLength( body );
    this->header["Content-Type"] = this->mime[".txt"];
    this->header.erase("Transfer-Encoding");
    response += this->httpVersion + " 201 " + this->statusCodes["201"] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += body;
    write(fd, response.c_str(), response.size());
    this->isResponseSent = true;
}

void Response::methodDelete( size_t fd ) {
    (void)fd;
}

bool Response::getIsResponseSent() {
    return (this->isResponseSent);
}

void Response::setTargetFile() {
    this->targetFile.open(this->request->getRequestTarget(), std::ios::in);
}

std::string &Response::getErrorCode() {
    return (this->errorCode);
}

void Response::resetAttributes() {
    this->isHeaderSent = false;
    this->isResponseSent = false;
    // targetFile.close();
}

void Response::setSocket( Socket *socket ) {
    this->socket = socket;
}

Response::~Response (void) {
    
}
