#pragma once
#include "stdafx.hpp"

inline size_t WriteCallback(char* buffer, size_t size, size_t nitems, void* data) {
    u64 totalSize{ size * nitems };
    reinterpret_cast<std::string*>(data)->append(buffer, totalSize);
    return totalSize;
}
class CurlWrapper {
public:
    CurlWrapper() {
        curl = curl_easy_init();
    }
    ~CurlWrapper() {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    template <typename ...a>
    bool SetEasyOpt(CURLoption option, a&&... args) {
        return curl_easy_setopt(curl, option, args...);
    }
    bool SetUrl(const std::string& url) {
        return SetEasyOpt(CURLOPT_URL, url.c_str());
    }
    bool SetFollowLocation(bool follow) {
        return SetEasyOpt(CURLOPT_FOLLOWLOCATION, follow);
    }
    bool SetHeader(const std::string& header) {
        m_headers = curl_slist_append(m_headers, header.c_str());
        return m_headers;
    }
    bool SetHeaders(const std::vector<std::string>& headers) {
        for (const auto& header : headers) {
            SetHeader(header);
        }
        return SetEasyOpt(CURLOPT_HTTPHEADER, m_headers);
    }
    bool SetPostFields(const std::string& fields) {
        if (bool result{ SetEasyOpt(CURLOPT_POSTFIELDS, fields.c_str()) }) {
            return SetEasyOpt(CURLOPT_POSTFIELDSIZE, fields.length());
        }
        return false;
    }
    bool SetWriteFunction(curl_write_callback callback) {
        return SetEasyOpt(CURLOPT_WRITEFUNCTION, callback);
    }
    bool SetWriteData(void* data) {
        return SetEasyOpt(CURLOPT_WRITEDATA, data);
    }
    bool Perform() {
        CURLcode result{ curl_easy_perform(curl) };
        if (result != CURLE_OK) {
            LOG_ERROR(std::format("cURL error : {}", curl_easy_strerror(result)).c_str());
            return false;
        }
        return true;
    }
    std::string Post(const std::string& url, const std::string& data, const std::vector<std::string>& headers = {}) {
        SetUrl(url);
        SetEasyOpt(CURLOPT_POST, true);
        SetFollowLocation(true);
        SetHeaders(headers);
        SetPostFields(data);
        SetWriteFunction(WriteCallback);
        SetWriteData(&responseBody);
        if (!Perform()) {
            return "bad_res";
        }
        return responseBody;
    }
private:
    CURL* curl;
    struct curl_slist* m_headers{};
    std::string responseBody{};
    std::string responseHeaders{};
};