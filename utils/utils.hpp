/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:05:26 by zaelarb           #+#    #+#             */
/*   Updated: 2025/03/04 15:05:28 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <map>
#include <iostream>

void ft_split(std::string& line, std::vector<std::string>& parts);
bool ft_isdigits(std::string& number);