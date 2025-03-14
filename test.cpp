#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <cstdlib>

void handleCGIRequest(const std::string &requestLine) {
    // Example: requestLine = "GET /cgi-bin/hello.cgi?name=John HTTP/1.1"

    // Extract script path and query string
    size_t queryPos = requestLine.find('?');
    size_t spacePos = requestLine.find(' ', 4); // Find space after "GET "

    std::string scriptPath;
    std::string queryString;

    if (queryPos != std::string::npos) {
        scriptPath = requestLine.substr(4, queryPos - 4);
        queryString = requestLine.substr(queryPos + 1, spacePos - queryPos - 1);
    } else {
        scriptPath = requestLine.substr(4, spacePos - 4);
    }

    std::cout << "Script Path: " << scriptPath << "\n";
    std::cout << "Query String: " << queryString << "\n";

    // Convert script path to absolute path (adjust for your server setup)
    std::string fullScriptPath = "/usr/lib/cgi-bin" + scriptPath.substr(8);
    std::cout << "QfullScriptPath: " << fullScriptPath << "\n";

    // Set environment variable for CGI
    setenv("QUERY_STRING", queryString.c_str(), 1);

    // Execute CGI script
    char *argv[] = {const_cast<char *>(fullScriptPath.c_str()), NULL};
    char *envp[] = {const_cast<char *>(("QUERY_STRING=" + queryString).c_str()), NULL};

    execve(fullScriptPath.c_str(), argv, envp);

    // If execve fails
    perror("execve");
}

int main() {
    std::string requestLine = "GET /cgi-bin/hello.cgi?name=John HTTP/1.1";

    handleCGIRequest(requestLine);

    return 0;
}
