/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup2.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:13:16 by momari            #+#    #+#             */
/*   Updated: 2025/01/08 15:27:29 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>

// In this example we open file and we replace this file with stdout file 
// We notice that when we call std::cout the result shown in the file not the in stdout

int main(int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "usage ./a.out file_name" << std::endl;
        return (1);
    }
    int outfile = open(av[1], O_CREAT | O_RDWR , 0777);
    if (outfile == -1) {
        std::cerr <<  "open file error" << std::endl;
        return (1);
    }
    dup2(outfile, STDOUT_FILENO);
    std::cout << "Hello world !!" << std::endl;
    close(outfile);
    return (0);
}