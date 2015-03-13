#pragma once

#include <string>

#include "./typedefs.h"

class HttpClient {
public:
    /* Initially set the CURL instance to nullptr */
    HttpClient();
    /* Construct the CURL instance with curl_easy_init */
    void init();
    /* Get a webpage's HTML */
    std::string& get(const std::string& url);

private:
    /* CURL helpers */
    template <typename Options, typename ThirdArg>
    void easySetopt(Options options, const ThirdArg& arg) const;
    void easyPerform() const;
    static size_t writeCallback(const void* contents, size_t size, size_t nmemb, void* userp);
    /* Members */
    CurlManaged m_curl;
    std::string m_buffer;
};
