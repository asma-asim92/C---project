#ifndef OPEN_LIBRARY_API_H
#define OPEN_LIBRARY_API_H

#include "Book.h"
#include <string>
#include <vector>

// Uses internet + JSON (nlohmann/json external module) via Open Library Search API
namespace OpenLibraryApi {
    // Search Open Library and return Book objects (ids left at 0; Library assigns them)
    std::vector<Book> searchBooks(const std::string& query, int limit = 5);

    // Parse a raw Open Library search.json response into Book objects
    std::vector<Book> parseSearchJson(const std::string& jsonText);
}

#endif
