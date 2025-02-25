/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:03:33 by momari            #+#    #+#             */
/*   Updated: 2025/02/23 21:41:59 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {
    // 1xx: Informational responses
    this->statusCodes["100"] = "Continue";
    this->statusCodes["101"] = "Switching Protocols";
    this->statusCodes["102"] = "Processing";
    this->statusCodes["103"] = "Early Hints";

    // 2xx: Successful responses
    this->statusCodes["200"] = "OK";
    this->statusCodes["201"] = "Created";
    this->statusCodes["202"] = "Accepted";
    this->statusCodes["203"] = "Non-Authoritative Information";
    this->statusCodes["204"] = "No Content";
    this->statusCodes["205"] = "Reset Content";
    this->statusCodes["206"] = "Partial Content";
    this->statusCodes["207"] = "Multi-Status";
    this->statusCodes["208"] = "Already Reported";
    this->statusCodes["226"] = "IM Used";

    // 3xx: Redirection messages
    this->statusCodes["300"] = "Multiple Choices";
    this->statusCodes["301"] = "Moved Permanently";
    this->statusCodes["302"] = "Found";
    this->statusCodes["303"] = "See Other";
    this->statusCodes["304"] = "Not Modified";
    this->statusCodes["305"] = "Use Proxy";
    this->statusCodes["306"] = "(Unused)";
    this->statusCodes["307"] = "Temporary Redirect";
    this->statusCodes["308"] = "Permanent Redirect";

    // 4xx: Client error responses
    this->statusCodes["400"] = "Bad Request";
    this->statusCodes["401"] = "Unauthorized";
    this->statusCodes["402"] = "Payment Required";
    this->statusCodes["403"] = "Forbidden";
    this->statusCodes["404"] = "Not Found";
    this->statusCodes["405"] = "Method Not Allowed";
    this->statusCodes["406"] = "Not Acceptable";
    this->statusCodes["407"] = "Proxy Authentication Required";
    this->statusCodes["408"] = "Request Timeout";
    this->statusCodes["409"] = "Conflict";
    this->statusCodes["410"] = "Gone";
    this->statusCodes["411"] = "Length Required";
    this->statusCodes["412"] = "Precondition Failed";
    this->statusCodes["413"] = "Content Too Large";
    this->statusCodes["414"] = "URI Too Long";
    this->statusCodes["415"] = "Unsupported Media Type";
    this->statusCodes["416"] = "Range Not Satisfiable";
    this->statusCodes["417"] = "Expectation Failed";
    this->statusCodes["418"] = "I'm a teapot";
    this->statusCodes["421"] = "Misdirected Request";
    this->statusCodes["422"] = "Unprocessable Entity";
    this->statusCodes["423"] = "Locked";
    this->statusCodes["424"] = "Failed Dependency";
    this->statusCodes["425"] = "Too Early";
    this->statusCodes["426"] = "Upgrade Required";
    this->statusCodes["428"] = "Precondition Required";
    this->statusCodes["429"] = "Too Many Requests";
    this->statusCodes["431"] = "Request Header Fields Too Large";
    this->statusCodes["451"] = "Unavailable For Legal Reasons";

    // 5xx: Server error responses
    this->statusCodes["500"] = "Internal Server Error";
    this->statusCodes["501"] = "Not Implemented";
    this->statusCodes["502"] = "Bad Gateway";
    this->statusCodes["503"] = "Service Unavailable";
    this->statusCodes["504"] = "Gateway Timeout";
    this->statusCodes["505"] = "HTTP Version Not Supported";
    this->statusCodes["506"] = "Variant Also Negotiates";
    this->statusCodes["507"] = "Insufficient Storage";
    this->statusCodes["508"] = "Loop Detected";
    this->statusCodes["510"] = "Not Extended";
    this->statusCodes["511"] = "Network Authentication Required";
}

HttpResponse::~HttpResponse() {
    
}