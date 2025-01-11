/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 10:15:54 by momari            #+#    #+#             */
/*   Updated: 2025/01/08 12:37:14 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

// in this example i have to run ls -la commant using execve funtion.
// the result will print in stdout

int main()
{
    char pathname[]         = "/bin/ls";
    const char *argv[10]    = {"ls", "-la", 0};

    execve(pathname, const_cast<char* const*>(argv), NULL);
}