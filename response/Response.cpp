/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaelarb <zaelarb@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:49:06 by momari            #+#    #+#             */
/*   Updated: 2025/02/08 16:42:03 by zaelarb          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Response::setMime() {
    this->mime[".aac"]    = "audio/aac";
    this->mime[".abw"]    = "application/x-abiword";
    this->mime[".apng"]   = "image/apng";
    this->mime[".arc"]    = "application/x-freearc";
    this->mime[".avif"]   = "image/avif";
    this->mime[".avi"]    = "video/x-msvideo";
    this->mime[".azw"]    = "application/vnd.amazon.ebook";
    this->mime[".bin"]    = "application/octet-stream";
    this->mime[".bmp"]    = "image/bmp";
    this->mime[".bz"]     = "application/x-bzip";
    this->mime[".bz2"]    = "application/x-bzip2";
    this->mime[".cda"]    = "application/x-cdf";
    this->mime[".csh"]    = "application/x-csh";
    this->mime[".css"]    = "text/css";
    this->mime[".csv"]    = "text/csv";
    this->mime[".doc"]    = "application/msword";
    this->mime[".docx"]   = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    this->mime[".eot"]    = "application/vnd.ms-fontobject";
    this->mime[".epub"]   = "application/epub+zip";
    this->mime[".gz"]     = "application/gzip";
    this->mime[".gif"]    = "image/gif";
    this->mime[".htm"]    = "text/html";
    this->mime[".html"]   = "text/html";
    this->mime[".ico"]    = "image/vnd.microsoft.icon";
    this->mime[".ics"]    = "text/calendar";
    this->mime[".jar"]    = "application/java-archive";
    this->mime[".jpeg"]   = "image/jpeg";
    this->mime[".jpg"]    = "image/jpeg";
    this->mime[".js"]     = "text/javascript";
    this->mime[".json"]   = "application/json";
    this->mime[".jsonld"] = "application/ld+json";
    this->mime[".mid"]    = "audio/midi";
    this->mime[".midi"]   = "audio/midi";
    this->mime[".mjs"]    = "text/javascript";
    this->mime[".mp3"]    = "audio/mpeg";
    this->mime[".mp4"]    = "video/mp4";
    this->mime[".mpeg"]   = "video/mpeg";
    this->mime[".mpkg"]   = "application/vnd.apple.installer+xml";
    this->mime[".odp"]    = "application/vnd.oasis.opendocument.presentation";
    this->mime[".ods"]    = "application/vnd.oasis.opendocument.spreadsheet";
    this->mime[".odt"]    = "application/vnd.oasis.opendocument.text";
    this->mime[".oga"]    = "audio/ogg";
    this->mime[".ogv"]    = "video/ogg";
    this->mime[".ogx"]    = "application/ogg";
    this->mime[".opus"]   = "audio/ogg";
    this->mime[".otf"]    = "font/otf";
    this->mime[".png"]    = "image/png";
    this->mime[".pdf"]    = "application/pdf";
    this->mime[".php"]    = "application/x-httpd-php";
    this->mime[".ppt"]    = "application/vnd.ms-powerpoint";
    this->mime[".pptx"]   = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    this->mime[".rar"]    = "application/vnd.rar";
    this->mime[".rtf"]    = "application/rtf";
    this->mime[".sh"]     = "application/x-sh";
    this->mime[".svg"]    = "image/svg+xml";
    this->mime[".tar"]    = "application/x-tar";
    this->mime[".tif"]    = "image/tiff";
    this->mime[".tiff"]   = "image/tiff";
    this->mime[".ts"]     = "video/mp2t";
    this->mime[".ttf"]    = "font/ttf";
    this->mime[".txt"]    = "text/plain";
    this->mime[".vsd"]    = "application/vnd.visio";
    this->mime[".wav"]    = "audio/wav";
    this->mime[".weba"]   = "audio/webm";
    this->mime[".webm"]   = "video/webm";
    this->mime[".webp"]   = "image/webp";
    this->mime[".woff"]   = "font/woff";
    this->mime[".woff2"]  = "font/woff2";
    this->mime[".xhtml"]  = "application/xhtml+xml";
    this->mime[".xls"]    = "application/vnd.ms-excel";
    this->mime[".xlsx"]   = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    this->mime[".xml"]    = "application/xml";
    this->mime[".xul"]    = "application/vnd.mozilla.xul+xml";
    this->mime[".zip"]    = "application/zip";
    this->mime[".3gp"]    = "video/3gpp";
    this->mime[".3g2"]    = "video/3gpp2";
    this->mime[".7z"]     = "application/x-7z-compressed";
}

Response::Response (  int fd, Request *request  ) {
    this->request = request;
    this->fd = fd;
    this->httpVersion = "HTTP/1.1";
    this->description["200"] = "OK";
    this->description["204"] = "No Content";
    Response::setMime();
    this->header["Server"] = "momari-zaelarb";
    std::cout << this->request->getRequestLine()->getRequestTarget().length() << std::endl;
    std::cout << this->request->getRequestLine()->getRequestTarget().substr(this->request->getRequestLine()->getRequestTarget().find_last_of(".")) << std::endl;
    std::map<std::string, std::string>::iterator it = this->mime.find(this->request->getRequestLine()->getRequestTarget().substr(this->request->getRequestLine()->getRequestTarget().find_last_of(".")));
    if (it != this->mime.end())
        this->header["Content-Type"] = it->second + "; charset=UTF-8";
}

Response::~Response (void) {
    
}

void Response::makeResponse ( void ) {
    if (this->request->getRequestLine()->getMethod() == "GET")
        methodGet();
    else if (this->request->getRequestLine()->getMethod() == "POST")
        methodPost();
    else if (this->request->getRequestLine()->getMethod() == "DELETE")
        methodDelete();
}

void Response::methodGet() {
    std::cout << "we are here" << this->header["Content-Type"] << std::endl;
    std::string response;
    std::string line;
    
    std::fstream targetFile( "." + this->request->getRequestLine()->getRequestTarget() );
    while (getline(targetFile, line)) {
        this->content += line + "\n";
    }
    response += this->httpVersion + " 200 " + this->description["200"] + CRLF;
    for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); it++)
        response += it->first + ": " + it->second + CRLF;
    response += CRLF;
    response += this->content;
    write(this->fd, response.c_str(), response.size());
}

void Response::methodPost() {
    
}

void Response::methodDelete() {
    
}