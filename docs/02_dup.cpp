/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:39:49 by momari            #+#    #+#             */
/*   Updated: 2025/01/08 13:09:02 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>
#include <cstring>

// In this example we take the output file (1) and create a copy of it and store it in new_fd variable
// After that i write "hello world !!\n" to the new_fd
// The output is :
// The new fd is : 3
// hello world !!

int main()
{
    const char *string = "hello world !!\n";
    int new_fd = dup(1);

    std::cout << "The new fd is : " << new_fd << std::endl;
    ssize_t n = write(new_fd, string, strlen(string));

    return (0);
}