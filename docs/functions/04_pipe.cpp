/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_pipe.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:33:42 by momari            #+#    #+#             */
/*   Updated: 2025/01/08 15:40:36 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>

int main() {
    int pipefd[2];

    int status = pipe(pipefd);
    if (status == -1) {
        std::cerr << "Pipe creation failed\n";
        return (1);
    }
    
    char buffer[100];
    write(pipefd[1], "Hello world", 11);
    read(pipefd[0], buffer, 11);

    std::cout << buffer << std::endl;
    close(pipefd[0]);
    close(pipefd[1]);
    return 0;
}