/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:26:11 by zaelarb           #+#    #+#             */
/*   Updated: 2025/02/19 20:40:07 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <exception>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/event.h>
#include <fcntl.h>
#include <algorithm>



class Server;
class ConfigFile;
class Location;
class Socket;
class Client;
class Response;

#include "../configFile/ConfigFile.hpp"
#include "../response/Response.hpp"
#include "../socket/Socket.hpp"
#include "../errorHandling/ErrorHandling.hpp"
#include "../server/Server.hpp"
#include "../configFile/Location.hpp"
#include "../error/Error.hpp"
#include "../client/Client.hpp"

std::vector<std::string> ft_split(std::string& line);
bool ft_isdigits(std::string& str);
std::vector<ConfigFile> parseConfigFile(std::string file);
void resolveLines(std::string &configContent);
void replaceTab(std::string& configContent);
void removeComments(std::string &configContent);
std::string cutConfigFile(std::string& configFile);
int endOfServer(std::string& content, int index);
