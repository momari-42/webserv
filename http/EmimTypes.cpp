/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EmimTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momari <momari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:05:56 by momari            #+#    #+#             */
/*   Updated: 2025/02/23 17:13:14 by momari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EmimTypes.hpp"

EmimTypes::EmimTypes() {
    this->emim["audio/aac"]                                                                 = ".aac"    ;
    this->emim["application/x-abiword"]                                                     = ".abw"    ;
    this->emim["image/apng"]                                                                = ".apng"   ;
    this->emim["application/x-freearc"]                                                     = ".arc"    ;
    this->emim["image/avif"]                                                                = ".avif"   ;
    this->emim["video/x-msvideo"]                                                           = ".avi"    ;
    this->emim["application/vnd.amazon.ebook"]                                              = ".azw"    ;
    this->emim["application/octet-stream"]                                                  = ".bin"    ;
    this->emim["image/bmp"]                                                                 = ".bmp"    ;
    this->emim["application/x-bzip"]                                                        = ".bz"     ;
    this->emim["application/x-bzip2"]                                                       = ".bz2"    ;
    this->emim["application/x-cdf"]                                                         = ".cda"    ;
    this->emim["application/x-csh"]                                                         = ".csh"    ;
    this->emim["text/css"]                                                                  = ".css"    ;
    this->emim["text/csv"]                                                                  = ".csv"    ;
    this->emim["application/msword"]                                                        = ".doc"    ;
    this->emim["application/vnd.openxmlformats-officedocument.wordprocessingml.document"]   = ".docx"   ;
    this->emim["application/vnd.ms-fontobject"]                                             = ".eot"    ;
    this->emim["application/epub+zip"]                                                      = ".epub"   ;
    this->emim["application/gzip"]                                                          = ".gz"     ;
    this->emim["image/gif"]                                                                 = ".gif"    ;
    this->emim["text/html"]                                                                 = ".htm"    ;
    this->emim["text/html"]                                                                 = ".html"   ;
    this->emim["image/vnd.microsoft.icon"]                                                  = ".ico"    ;
    this->emim["text/calendar"]                                                             = ".ics"    ;
    this->emim["application/java-archive"]                                                  = ".jar"    ;
    this->emim["image/jpeg"]                                                                = ".jpeg"   ;
    this->emim["image/jpeg"]                                                                = ".jpg"    ;
    this->emim["text/javascript"]                                                           = ".js"     ;
    this->emim["application/json"]                                                          = ".json"   ;
    this->emim["application/ld+json"]                                                       = ".jsonld" ;
    this->emim["audio/midi"]                                                                = ".mid"    ;
    this->emim["audio/midi"]                                                                = ".midi"   ;
    this->emim["text/javascript"]                                                           = ".mjs"    ;
    this->emim["audio/mpeg"]                                                                = ".mp3"    ;
    this->emim["video/mp4"]                                                                 = ".mp4"    ;
    this->emim["video/mpeg"]                                                                = ".mpeg"   ;
    this->emim["application/vnd.apple.installer+xml"]                                       = ".mpkg"   ;
    this->emim["application/vnd.oasis.opendocument.presentation"]                           = ".odp"    ;
    this->emim["application/vnd.oasis.opendocument.spreadsheet"]                            = ".ods"    ;
    this->emim["application/vnd.oasis.opendocument.text"]                                   = ".odt"    ;
    this->emim["audio/ogg"]                                                                 = ".oga"    ;
    this->emim["video/ogg"]                                                                 = ".ogv"    ;
    this->emim["application/ogg"]                                                           = ".ogx"    ;
    this->emim["audio/ogg"]                                                                 = ".opus"   ;
    this->emim["font/otf"]                                                                  = ".otf"    ;
    this->emim["image/png"]                                                                 = ".png"    ;
    this->emim["application/pdf"]                                                           = ".pdf"    ;
    this->emim["application/x-httpd-php"]                                                   = ".php"    ;
    this->emim["application/vnd.ms-powerpoint"]                                             = ".ppt"    ;
    this->emim["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = ".pptx"   ;
    this->emim["application/vnd.rar"]                                                       = ".rar"    ;
    this->emim["application/rtf"]                                                           = ".rtf"    ;
    this->emim["application/x-sh"]                                                          = ".sh"     ;
    this->emim["image/svg+xml"]                                                             = ".svg"    ;
    this->emim["application/x-tar"]                                                         = ".tar"    ;
    this->emim["image/tiff"]                                                                = ".tif"    ;
    this->emim["image/tiff"]                                                                = ".tiff"   ;
    this->emim["video/mp2t"]                                                                = ".ts"     ;
    this->emim["font/ttf"]                                                                  = ".ttf"    ;
    this->emim["text/plain"]                                                                = ".txt"    ;
    this->emim["application/vnd.visio"]                                                     = ".vsd"    ;
    this->emim["audio/wav"]                                                                 = ".wav"    ;
    this->emim["audio/webm"]                                                                = ".weba"   ;
    this->emim["video/webm"]                                                                = ".webm"   ;
    this->emim["image/webp"]                                                                = ".webp"   ;
    this->emim["font/woff"]                                                                 = ".woff"   ;
    this->emim["font/woff2"]                                                                = ".woff2"  ;
    this->emim["application/xhtml+xml"]                                                     = ".xhtml"  ;
    this->emim["application/vnd.ms-excel"]                                                  = ".xls"    ;
    this->emim["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"]         = ".xlsx"   ;
    this->emim["application/xml"]                                                           = ".xml"    ;
    this->emim["application/vnd.mozilla.xul+xml"]                                           = ".xul"    ;
    this->emim["application/zip"]                                                           = ".zip"    ;
    this->emim["video/3gpp"]                                                                = ".3gp"    ;
    this->emim["video/3gpp2"]                                                               = ".3g2"    ;
    this->emim["application/x-7z-compressed"]                                               = ".7z"     ;
}

EmimTypes::~EmimTypes() {
}