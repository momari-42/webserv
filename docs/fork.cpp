/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:12:55 by momari            #+#    #+#             */
/*   Updated: 2025/01/08 17:12:56 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "Fork failed\n";
    } else if (pid == 0) {
        std::cout << "Child process\n";
    } else {
        std::cout << "Parent process, child pid: " << pid << "\n";
    }
    return 0;
}