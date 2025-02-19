/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:26:19 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/19 11:48:58 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sources.hpp"

std::vector<std::string> ft_split(std::string& line) {
    std::vector<std::string> words;
    std::string tmp;

    line.erase(0, line.find_first_not_of(' '));
    while (line.length() > 0)
    {
        if (line.find(' ') < line.find(';')) {
            tmp = line.substr(0, line.find(' '));
            line.erase(0, line.find(' '));
        }else {
            tmp = line.substr(0, line.find(';'));
            line.erase(0, line.find(';'));
        }
        line.erase(0, line.find_first_not_of(' '));
        // std::cout << tmp << std::endl;
        if (tmp.compare("\n"))
            words.push_back(tmp);
        if (line[0] == ';') {
            line.erase(0, 1);
            break;
        }
    }
    return words;
}

bool ft_isdigits(std::string& str) {
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

int endOfServer(std::string& content, int index) {
    size_t count = 1;
    while ((unsigned long)index < content.length()) {
        if (content[index] == '}')
         count--;
        else if (content[index] == '{')
         count++;
        if (count == 0)
            return index;
        index++;
    }
    return -1;
}

std::string cutConfigFile(std::string& configFile) {
    size_t index = 0;
    while (configFile[index] == ' ' || configFile[index] == '\n' || configFile[index] == '\t')
        index++;
    if (configFile.length() == index)
        return "";
    if (configFile.find("server") != index) {
        std::cerr << "Something wrong in the config file" << std::endl;
        return "";
    }
    index += 6;
    while (configFile[index] == ' ' || configFile[index] == '\n' || configFile[index] == '\t')
        index++;
    if (configFile[index] != '{') {
        std::cerr << "Something wrong in the config file" << std::endl;
        return "";
    }
    int start = index + 1;
    int end = endOfServer(configFile, start);
    std::string serverInfo = configFile.substr(start, end - start);
    configFile.erase(0, end + 1);
    return serverInfo;
}

void removeComments(std::string &configContent) {
    while (configContent.find('#') != std::string::npos) {
        configContent.erase(configContent.find('#'), configContent.find('\n', configContent.find('#')) - configContent.find('#'));
    }
}

void replaceTab(std::string& configContent) {
    for (size_t i = 0; i < configContent.length(); i++)
    {
        if (configContent[i] == '\t')
            configContent[i] = ' ';
    }
}

void resolveLines(std::string &configContent) {
    for (size_t i = 0; i < configContent.length(); i++)
    {
        if ((configContent[i] == ';' || configContent[i] == '{' \
            || configContent[i] == '}') && configContent[i + 1] != '\n')
            configContent.insert(configContent.begin() + i + 1, '\n');
    }
}

std::vector<ConfigFile> parseConfigFile(std::string file) {
    std::vector<ConfigFile> servers;
    if (access(file.c_str(), R_OK) == -1) {
        std::cerr << "Can't access to the config file" << std::endl;
        return servers;
    }
    std::fstream config(file);
    std::string configContent;
    std::string line;
    while (getline(config, line)) {
        configContent += line + "\n";
    }
    removeComments(configContent);
    replaceTab(configContent);
    resolveLines(configContent);
    for (size_t i = 0; i < 3; i++)
    {
        std::string serverInfo = cutConfigFile(configContent);
        if (serverInfo.length() == 0)
            break;
        ConfigFile server;
        server.parseServerInfo(serverInfo);
        server.showServerConfig();
        servers.push_back(server);
    }
    return servers;
}
