#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>

// External module: WinHTTP (Windows) used to fetch data from the internet
namespace HttpClient {
    // GET a URL and return the response body. Throws std::runtime_error on failure.
    std::string get(const std::string& url);
}

#endif
