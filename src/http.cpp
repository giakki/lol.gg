#include "./http.h"

#include <curl/curl.h>

#include <stdexcept>

HttpClient::HttpClient() : m_curl { nullptr, [](CURL* c) { if (c != nullptr) { curl_easy_cleanup(c); }} } {
}

void HttpClient::init() {
    m_curl.reset(curl_easy_init());
    if (m_curl == nullptr) {
        throw std::runtime_error("Error initializing HttpClient");
    }
}

std::string& HttpClient::get(const std::string& url) {
    /* Reset buffer size. Does not reallocate entire string. */
    m_buffer.clear();
    /* Perform request */
    easySetopt(CURLOPT_URL, url.c_str());
    easySetopt(CURLOPT_FOLLOWLOCATION, 1L);
    easySetopt(CURLOPT_WRITEFUNCTION, &HttpClient::writeCallback);
    easySetopt(CURLOPT_WRITEDATA, &m_buffer);
    easyPerform();

    return m_buffer;
}

template <typename Options, typename ThirdArg>
void HttpClient::easySetopt(Options options, const ThirdArg& arg) const {
    auto res = curl_easy_setopt(m_curl.get(), options, arg);
    if (res != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(res));
    }
}

void HttpClient::easyPerform() const {
    auto res = curl_easy_perform(m_curl.get());
    if (res != CURLE_OK) {
        throw std::runtime_error(curl_easy_strerror(res));
    }
}

size_t HttpClient::writeCallback(const void* contents, size_t size, size_t nmemb, void* userp) {
    static_cast<std::string*>(userp)->append(static_cast<const char*>(contents), size * nmemb);
    return size * nmemb;
}
