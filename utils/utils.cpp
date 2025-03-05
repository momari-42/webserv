/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:05:12 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/04 15:05:15 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void ft_split(std::string& line, std::vector<std::string>& parts) {
    std::string word;
    while (line.size()) {
        line.erase(0, line.find_first_not_of(" \n;"));
        if (!line.size())
            break;
        word = line.substr(0, line.find_first_of(" \n;"));
        line.erase(0, line.find_first_of(" \n;"));
        parts.push_back(word);
    }
}

bool ft_isdigits(std::string& number) {
    for (size_t i = 0; i < number.size(); i++)
    {
        if (!isdigit(number[i]))
            return false;
    }
    return true;
    
}