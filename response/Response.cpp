/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:06 by momari            #+#    #+#             */
/*   Updated: 2025/03/21 01:33:37 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string generateSessionID() {
    std::stringstream ss;
    ss << std::hex << (rand() % 0xFFFFFFF); // Generate a random hex string
    return ss.str();
}

Response::Response ( Request *request, bool &isReadyForNextRequest ) : isReadyForNextRequest(isReadyForNextRequest) {
    this->request = request;
    initiatConfigFile = false;
    this->isHeaderSent      = false;
    this->isResponseSent    = false;
    this->isCgiComplet      = false;
    this->listingDirectory  = false;
    this->httpVersion       = "HTTP/1.1";
    this->fd[0]             = -2;
    this->fd[1]             = -2;
    this->inout[0]          = -2;
    this->inout[1]          = -2;
    this->fdClient          = -2;

    this->header["Server"] = "momari-zaelarb";
    this->header["Connection"] = "keep-alive";
    this->header["Transfer-Encoding"] = "chunked";

    srand(time(NULL));
    this->sessionID = generateSessionID();
    (void)this->isReadyForNextRequest;
}

void freeEnvSpaces(char **env) {
    int index = 0;
    while (env[index]) {
        if (index >= 3)
            free(env[index]);
        index++;
    }
}

void Response::executeCGI ( size_t fd, size_t kq ) {
    std::map<std::string, std::string> httpHeadersMap = this->request->getHeader()->getHttpHeadersMap();
    char                               *env[httpHeadersMap.size() + 4];
    char                               *argv[3];
    std::string varValue        = this->request->getRoot();
    std::string method          = this->request->getRequestLine()->getMethod();
    std::string requestTarget   = this->request->getRequestTarget();
    std::string queryString;
    
    this->fdClient = fd;
    if (this->request->getRequestLine()->getMethod() == "POST") {
        std::string inFile = this->request->getRandomeFileName();
        int fd = open(inFile.c_str(), O_RDONLY);
        unlink(inFile.c_str());
        dup2(fd, 0);
        close(fd);
    }
    queryString = "QUERY_STRING=";
    queryString += this->queryString;
    // std::cerr << "this is the script --->>>>: " << this->request->getLocation().cgi[this->request->getCgiExtention()].c_str() << std::endl;
    // std::cerr << "this is the script --->>>>: " << requestTarget.c_str() << std::endl;
    argv[0] = const_cast<char *>( this->request->getLocation().cgi[this->request->getCgiExtention()].c_str());
    argv[1] = const_cast<char *>( requestTarget.c_str());
    argv[2] = NULL;
    // this where i init the env;
    varValue = "UPLOAD_TMP_DIR=" + varValue;
    method   = "METHOD=" + method;
    setenv("QUERY_STRING", queryString.c_str(), 1);
    setenv("UPLOAD_TMP_DIR", varValue.c_str(), 1);
    setenv("METHOD", method.c_str(), 1);
    for (std::map<std::string, std::string>::iterator it = httpHeadersMap.begin(); it != httpHeadersMap.end(); it++ ) {
        setenv(it->first.c_str(), (std::string(it->first + "=" + it->second)).c_str(), 1);
    }
    env[0] = const_cast<char *>(queryString.c_str());
    env[1] = const_cast<char *>(varValue.c_str());
    env[2] = const_cast<char *>(method.c_str());
    size_t index = 3;
    for (std::map<std::string, std::string>::iterator it = httpHeadersMap.begin(); it != httpHeadersMap.end(); it++ ) {
        std::string tmp = it->first + "=" + it->second;
        env[index]= strdup(const_cast<char *>(tmp.c_str()));
        index++;
    }
    env[index] = 0;
    if (pipe(this->fd) == -1) {
        exit(1);
    }
    this->pid = fork();
    if (pid == 0) {
        close(this->fd[0]);
        dup2(this->fd[1], 1);
        close(this->fd[1]);
        if (execve(argv[0], argv, env) == -1) {
            freeEnvSpaces(env);
        // std::cerr << "0000000000000000000000000000000000000000000000000000000000000000000000000000" << std::endl;
            std::cerr << "lolo" << std::endl;
            exit(1);
        }
    }
    // usleep(200);
    freeEnvSpaces(env);
    struct kevent eventForDelete;
    struct kevent events[2];

    EV_SET(&eventForDelete, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &eventForDelete, 1, NULL, 0, NULL) == -1)
        throw (Response::ResponseExceptions(strerror(errno)));
    EV_SET(&events[0], this->pid, EVFILT_PROC, EV_ADD | EV_ENABLE, NOTE_EXIT, 0, &this->fdClient);
    EV_SET(&events[1], this->pid, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 10000, &this->fdClient);

    if (kevent(kq, events, 2, NULL, 0, NULL) == -1) {
        throw (Response::ResponseExceptions(strerror(errno)));
    }
    this->isCgiComplet = true;
}

void Response::setServerCookies() {
    std::ifstream inFile("cookie/" + this->sessionID);
    if (inFile.is_open()) {
        std::string line;
        while (getline(inFile, line)) {
            std::string value;
            std::string key = line.substr(0, line.find(":"));
            line.erase(0, line.find(":") + 1);
            value = line.substr(0);
            this->request->cookies[key] = value;
        }
    }
    std::string cookiesLine = this->request->getRequestLine()->getData();
    while (cookiesLine.size()) {
        std::string value;
        std::string key = cookiesLine.substr(0, cookiesLine.find("="));
        cookiesLine.erase(0, cookiesLine.find("=") + 1);
        if (cookiesLine.find("&") != std::string::npos) {
            value = cookiesLine.substr(0, cookiesLine.find("&"));
            cookiesLine.erase(0, cookiesLine.find("&") + 1);
        } else {
            value = cookiesLine.substr(0);
            cookiesLine.erase(0);
        }
        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == '+')
                value[i] = ' ';
        }
        this->request->cookies[key] = value;
    }
    std::ofstream of("cookie/" + this->sessionID, std::ios::out);
    if (of.is_open()) {
        for (std::map<std::string, std::string>::iterator it = this->request->cookies.begin(); it != this->request->cookies.end(); it++)
            of << it->first + ":" + it->second + "\n";
    }
}

void Response::makeResponse ( size_t fd, size_t kq ) {
    if (this->request->cookies["session_id"].size()) {
        this->sessionID = this->request->cookies["session_id"];
    } else
        this->request->cookies["session_id"] = this->sessionID;
    if (access(("cookie/" + this->sessionID).c_str(), R_OK) == -1) {
        std::ofstream sessionFile("cookie/" + this->sessionID);
        sessionFile.close();
    }
    setServerCookies();
    if (!initiatConfigFile) {
        this->configFile =  this->socket->getServerConfig(this->request->getHeader()->getValue("HOST"));
        this->location =  this->configFile->getLocations()[this->configFile->getMatchedLocation()];
        if ( this->request->getCgi() ) {
            this->inout[0] = dup(0);
            this->inout[1] = dup(1);
        }
        size_t      queryPos        = this->request->getRequestTarget().find("?");
        if (queryPos != std::string::npos) {
            std::cout << "before : " << this->request->getRequestTarget()<< std::endl;
            this->queryString = this->request->getRequestTarget().substr(queryPos + 1);
            this->request->getRequestTarget().erase(queryPos);
            std::cout << "after  : " << this->queryString<< std::endl;
        }
        initiatConfigFile = true;
    }
    if (this->request->getCgi() && !this->isCgiComplet)
        executeCGI( fd, kq );
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
    if ( this->location.listing ) {
        std::string tmp = this->request->getRequestTarget();
        std::string toFind = this->request->getRoot();
        if (toFind.size() && toFind.at(toFind.size() - 1) == '/')
            toFind.erase(toFind.size() - 1);
        int n = 0;
        while (!tmp.find(toFind)) {
            tmp.erase(0, toFind.size());
            // std::cout << "this is :" << tmp << std::endl;
            n++;
        }
        if( n == 2 ) {
            // std::cout << "this is the request target after :" << this->request->getRequestTarget() << std::endl;
            this->request->getRequestTarget().erase(0, toFind.size());
        }   
        // std::cout << "this is the root :" << n << "-" << this->request->getRoot() << std::endl;
    }
    if ( isDirectory(this->request->getRequestTarget()) ) {
        bool isValidPath = false;
        std::vector<std::string> &index = this->configFile->getIndex();
        if (index.size()) {
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
        else if (this->location.listing) {
            listingDirectory = true;
        }
        else
            this->errorCode = "404" ;
        // std::cout << "this is where the 404 thrown " << std::endl;
    }
    else {
        if (access( this->request->getRequestTarget().c_str(), F_OK ) == -1) {
            // std::cout << "665556656556565 ->>>>" << this->request->getRequestTarget().c_str() << "<<<<<----" << std::endl;
            this->errorCode = "404";
            return;
        }   
        if (access( this->request->getRequestTarget().c_str(), R_OK ) == -1) {
            this->errorCode = "401";
            return;
        }
    }
}

void Response::sendDirectoryList( size_t fd ) {
    struct dirent   *entry;
    std::string response;
    std::string htmlContent = "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Directory Listing</title>\n"
        "</head>\n"
        "<body>\n"
        "<h1>Directory Listing From " + this->request->getRoot() + " </h1>"
        "    <div class=\"container\">\n"
        "        <nav class=\"nav\">\n"
        "            <ul>\n";
    DIR *dir = opendir(this->request->getRequestTarget().c_str());
    if (dir) {
        while ( (entry = readdir(dir)) ) {
            std::string &requestTarget = this->request->getRequestTarget();
            if ( requestTarget.size() && requestTarget.at(requestTarget.size() - 1) != '/')
                requestTarget.push_back('/');
            htmlContent += "<li><a href=\"" + this->request->getRequestTarget() + std::string(entry->d_name) + "\">";
            htmlContent += entry->d_name + std::string("</a></li>\n");
        }
        htmlContent += "            </ul>\n"
        "        </nav>\n"
        "    </div>\n"
        "</body>\n"
        "</html>\n";
        std::string hexaNumber = convertDecimalToHexaToString(htmlContent.size()); 
        response += hexaNumber + CRLF;
        response += htmlContent + CRLF;
        if (send(fd, response.c_str(), response.size(), 0) == -1) {
            std::cout << "Error sending data" << std::endl;
            // Handle send error
        }
        (void)entry;
        this->isResponseSent = true;
        return;
    }
    this->errorCode = "500";
}

void Response::sendRedirectionResponse( size_t fd, Location &location ) {
    std::string response;

    response += this->httpVersion + " " + location.redirection.first + " "  + this->statusCodes[location.redirection.first] + CRLF;
    response += "Location: " + location.redirection.second + CRLF + CRLF;
    if (send(fd, response.c_str(), response.size(), 0) == -1) {
        std::cerr << "Error sending data" << std::endl; 
    }
}

// void setCookie(std::string& response, std::string& sessionID) {
    
// }

void Response::generateHeader ( int fd, std::string &response) {
    Location &location =  this->request->getLocation();
    if (!location.redirection.first.empty() && !location.redirection.second.empty()) {
        sendRedirectionResponse(fd, location);
        this->isHeaderSent = true;
        this->isResponseSent = true;
        return;
    }
    if (this->request->getCgi())
        this->header["Content-Type"] = "text/html" + std::string("; charset=UTF-8");
    else {
        validateRequestTarget();
        if (this->errorCode.size())
            return ;
        if ( this->listingDirectory )
            this->header["Content-Type"] = "text/html; charset=UTF-8";
        else if (this->request->getRequestTarget().find(".") != std::string::npos ) {
            std::map<std::string, std::string>::iterator it = this->mime.find( \
                this->request->getRequestTarget().substr(    \
                    this->request->getRequestTarget().find_last_of(".")));
            if (it != this->mime.end()) {
                this->header["Content-Type"] = it->second + "; charset=UTF-8";
            }
        }
    }
    response += this->httpVersion + " 200 " + this->statusCodes["200"] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++) {
        if (it->first == "COOKIE")
            continue;
        response += it->first + ": " + it->second + CRLF;
    }
    for (std::map<std::string, std::string>::iterator it = this->request->cookies.begin(); it != this->request->cookies.end(); it++)
        response += "Set-Cookie: " + it->first + "=" + it->second + "; Path=" + this->request->getPath() + CRLF ;
    response += CRLF;
    // std::cout << response;
    if (send(fd, response.c_str(), response.size(), 0) == -1) {
        std::cerr << "Error sending data" << std::endl; 
    }
    if ( !this->request->getCgi() && !this->listingDirectory )
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

void Response::methodGet( size_t fd ) {
    std::string         response;
    char                buffer[BUFFER_SIZE_R];
    std::streamsize     bytesRead = 0;

    if (!this->isHeaderSent) {
        generateHeader(fd, response);
    }
    if ( this->isResponseSent || this->errorCode.size())
        return;
    memset(buffer, 0, sizeof(buffer));
    if ( this->listingDirectory )
        sendDirectoryList(fd);
    else if (this->request->getCgi()) {
        fcntl(this->fd[0], F_SETFL, O_NONBLOCK);
        bytesRead = read(this->fd[0], buffer, BUFFER_SIZE_R);
    }
    else {
        targetFile.read(buffer, BUFFER_SIZE_R);
        bytesRead = targetFile.gcount();
    }
    if (bytesRead > 0) {
        std::string content(buffer, bytesRead);
        std::string hexaNumber = convertDecimalToHexaToString(bytesRead); 
        response += hexaNumber + CRLF;
        response += content + CRLF;
        if (send(fd, response.c_str(), response.size(), 0) == -1) {
            std::cout << "Error sending data" << std::endl;
            // Handle send error
        }
        response.clear();
    }
    if (targetFile.eof() || bytesRead < BUFFER_SIZE_R) {
        response = "0" + std::string(CRLF) + CRLF;
        send(fd, response.c_str(), response.size(), 0);
        this->isResponseSent = true;
        if (this->request->getCgi()) {
            close (this->fd[0]);
            close (this->fd[1]);
        }
        else 
            targetFile.close(); // Close the file
    }
}


std::string calculateBodyLength( std::string &body ) {
    std::ostringstream size;

    size << body.size();
    return (size.str());
}

void Response::sendNoContentResponse( size_t fd ) {
    std::string response;
    std::string body = "No Content :(";

    if (this->request->getHeader()->getValue("CONNECTION") == "close") {
        this->header["Connection"] = "close";
    }
    this->header["Content-Length"] = calculateBodyLength( body );
    this->header["Content-Type"] = this->mime[".txt"];
    this->header.erase("Transfer-Encoding");
    response += this->httpVersion + " 204 " + this->statusCodes["204"] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += body;
    write(fd, response.c_str(), response.size());
    this->isResponseSent = true;
}

void Response::sendSuccessResponse( size_t fd ) {
    std::string response;
    std::string body;

    if (this->request->getHeader()->getValue("CONNECTION") == "close") {
        this->header["Connection"] = "close";
    }
    this->header["Content-Type"] = this->mime[".txt"];
    this->header.erase("Transfer-Encoding");
    if ( this->request->getBody()->getCreated() ) {
        response += this->httpVersion + " 201 " + this->statusCodes["201"] + CRLF;
        body = "File uploaded succefully !!";
    }
    else {
        response += this->httpVersion + " 200 " + this->statusCodes["200"] + CRLF;
        body = "No Content :( !!";
    }
    this->header["Content-Length"] = calculateBodyLength( body );
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += body;
    write(fd, response.c_str(), response.size());
    this->isResponseSent = true;
}

void Response::methodPost( size_t fd ) {
    if (this->request->getCgi())
        methodGet(fd);
    else
        sendSuccessResponse(fd);
    if (!this->request->getCgi())
        this->isResponseSent = true;
}

void Response::methodDelete( size_t fd ) {
    std::string path = this->request->getRequestTarget();
    if (access( path.c_str(), R_OK ) == -1 || isDirectory(path)) {
        // std::cout << "kjdsflkjsflds : " << path << std::endl;
        this->errorCode = "404";
        return;
    }
    if (unlink(path.c_str()) == -1) {
        std::string response;
        std::string body = "The file will delete by server later !!";

        if (this->request->getHeader()->getValue("CONNECTION") == "close") {
            this->header["Connection"] = "close";
        }
        this->header["Content-Length"] = calculateBodyLength( body );
        this->header["Content-Type"] = this->mime[".txt"];
        this->header.erase("Transfer-Encoding");
        response += this->httpVersion + " 202 " + this->statusCodes["202"] + CRLF;
        for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
            response += it->first + ": " + it->second + CRLF;
        response += CRLF;
        response += body;
        write(fd, response.c_str(), response.size());
        this->isResponseSent = true;
        return;
    } else {
        std::string response;
        std::string body = "File is deleted. !!";

        if (this->request->getHeader()->getValue("CONNECTION") == "close") {
            this->header["Connection"] = "close";
        }
        this->header["Content-Length"] = calculateBodyLength( body );
        this->header["Content-Type"] = this->mime[".txt"];
        this->header.erase("Transfer-Encoding");
        response += this->httpVersion + " 200 " + this->statusCodes["200"] + CRLF;
        for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
            response += it->first + ": " + it->second + CRLF;
        response += CRLF;
        response += body;
        write(fd, response.c_str(), response.size());
        this->isResponseSent = true;
    }
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
    this->isHeaderSent      = false;
    this->isResponseSent    = false;
    this->isCgiComplet      = false;
    this->listingDirectory  = false;
    this->initiatConfigFile = false;

    if (this->request->getCgi()) {
        close(this->fd[0]);
        close(this->fd[1]);
        
        dup2(this->inout[0], 0);
        dup2(this->inout[1], 1);
        close(this->inout[0]);
        close(this->inout[1]);
    }
    targetFile.close();
}

void Response::setSocket( Socket *socket ) {
    // std::cout << "I'm here body if YOU NEED ME!!" << std::endl;
    this->socket = socket;
}

Response::~Response (void) {
    if (this->request->getCgi()) {
        dup2(inout[0], 0);
        dup2(inout[1], 1);
        close(this->inout[0]);
        close(this->inout[1]);
    }
}


Response::ResponseExceptions::ResponseExceptions ( const std::string& errorMsg ) {
    this->errorMsg = errorMsg;
}

// Response::ResponseExceptions::~ResponseExceptions ( ) throw() {
// }

const char* Response::ResponseExceptions::what() const throw() {
    return (this->errorMsg.c_str());
}

void Response::setIsReadyForNextRequest ( bool isReadyForNextRequest ) {
    this->isReadyForNextRequest = isReadyForNextRequest;
}