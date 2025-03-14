/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:18:39 by momari            #+#    #+#             */
/*   Updated: 2025/01/09 22:35:22 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
}