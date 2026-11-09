#include "OpenLibraryApi.h"
#include "HttpClient.h"

#include <nlohmann/json.hpp>
#include <cctype>
#include <sstream>

using json = nlohmann::json;

namespace {
    std::string urlEncode(const std::string& value) {
        static const char* hex = "0123456789ABCDEF";
        std::string out;
        out.reserve(value.size() * 3);

        for (unsigned char c : value) {
            if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                out.push_back(static_cast<char>(c));
            } else if (c == ' ') {
                out.push_back('+');
            } else {
                out.push_back('%');
                out.push_back(hex[(c >> 4) & 0xF]);
                out.push_back(hex[c & 0xF]);
            }
        }
        return out;
    }
}

namespace OpenLibraryApi {

std::vector<Book> parseSearchJson(const std::string& jsonText) {
    std::vector<Book> books;

    json root = json::parse(jsonText);
    if (!root.contains("docs") || !root["docs"].is_array()) {
        return books;
    }

    // Loop over JSON array (collection from the internet)
    for (const json& doc : root["docs"]) {
        Book book;
        book.id = 0;
        book.title = doc.value("title", "Unknown Title");
        book.available = true;

        if (doc.contains("author_name") && doc["author_name"].is_array() &&
            !doc["author_name"].empty()) {
            book.author = doc["author_name"][0].get<std::string>();
        } else {
            book.author = "Unknown Author";
        }

        book.year = doc.value("first_publish_year", 0);
        books.push_back(book);
    }

    return books;
}

std::vector<Book> searchBooks(const std::string& query, int limit) {
    if (limit < 1) {
        limit = 1;
    }
    if (limit > 20) {
        limit = 20;
    }

    std::ostringstream url;
    url << "https://openlibrary.org/search.json?q=" << urlEncode(query)
        << "&limit=" << limit
        << "&fields=title,author_name,first_publish_year";

    const std::string response = HttpClient::get(url.str());
    return parseSearchJson(response);
}

}  // namespace OpenLibraryApi
