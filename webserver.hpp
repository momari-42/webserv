/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:45:41 by momari            #+#    #+#             */
/*   Updated: 2025/02/14 17:35:36 by momari           ###   ########.fr       */
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

// #include <cstring>
