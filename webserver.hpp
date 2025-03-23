/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:45:41 by momari            #+#    #+#             */
/*   Updated: 2025/03/23 01:18:37 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <signal.h>
#include <iostream>
#include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include "server/Server.hpp"
#include <csignal>

void validateFileExtention( std::string fileName );
void readConfigFile(std::fstream& configFile, std::string& file);
bool areClosed(std::string& file);
void resolveLine(std::string &line);
void openCookieDir();