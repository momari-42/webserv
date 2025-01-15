/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strerror.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:54:42 by momari            #+#    #+#             */
/*   Updated: 2025/01/08 16:22:17 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include  <cstring>
#include <iostream>

int main()
{
    // we pass invalid to open, this should return -1 and set the errno to indicate error
    int fd = open("invalid_path_name", O_RDWR);
    if (fd == -1) {
        std::cerr << strerror(errno) << std::endl;
        return (1);
    }
    std::cout << "Success !!" << std::endl;
}