/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:08 by momari            #+#    #+#             */
/*   Updated: 2025/02/06 20:38:28 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../request/requestLine/RequestLine.hpp"


class Response
{
    private:
        RequestLine *requestLine;


    public:
        Response( RequestLine *requestLine );
        ~Response();
        void makeResponse ();
};