#pragma once

#include <curl/curl.h>

#include <iosfwd>
#include <string>

#include "./typedefs.h"

/* Set console title on Windows */
void setTitle(const std::string& str);

/* Align string to center in console */
std::string alignCenter(const std::string& str);

/* Erase a console line */
std::ostream& clearLine(std::ostream& stream);

/* RAII function to manage gumbo resources */
GumboOutputManaged gumboParse(const char* html, size_t size);

/* curl_global_{init/cleanup} RAII wrapper */
struct CurlGlobal {
    CURLcode status = curl_global_init(CURL_GLOBAL_DEFAULT);
    ~CurlGlobal() { curl_global_cleanup(); }
};
