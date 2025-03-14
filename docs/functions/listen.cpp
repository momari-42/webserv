/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:28:19 by momari            #+#    #+#             */
/*   Updated: 2025/01/09 22:37:19 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <iostream>

// This is a simple usage of listen.

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}