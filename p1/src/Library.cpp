#include "Library.h"
#include "OpenLibraryApi.h"

#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace {
    std::string toLower(std::string s) {
        for (char& c : s) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        return s;
    }
}

Library::Library() : nextId_(1) {}

void Library::addBook(const Book& book) {
    Book copy = book;
    if (copy.id <= 0) {
        copy.id = nextId_++;
    } else if (copy.id >= nextId_) {
        nextId_ = copy.id + 1;
    }
    books_.push_back(copy);
}

bool Library::removeBook(int id) {
    // Loop + conditional: find and erase matching id
    for (auto it = books_.begin(); it != books_.end(); ++it) {
        if (it->id == id) {
            books_.erase(it);
            return true;
        }
    }
    return false;
}

Book* Library::findById(int id) {
    for (Book& book : books_) {
        if (book.id == id) {
            return &book;
        }
    }
    return nullptr;
}

std::vector<Book> Library::searchByTitle(const std::string& query) const {
    std::vector<Book> results;
    const std::string needle = toLower(query);

    for (const Book& book : books_) {
        if (toLower(book.title).find(needle) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

std::vector<Book> Library::searchByAuthor(const std::string& query) const {
    std::vector<Book> results;
    const std::string needle = toLower(query);

    for (const Book& book : books_) {
        if (toLower(book.author).find(needle) != std::string::npos) {
            results.push_back(book);
        }
    }
    return results;
}

bool Library::borrowBook(int id) {
    Book* book = findById(id);
    if (book == nullptr) {
        return false;
    }
    if (!book->available) {
        return false;
    }
    book->available = false;
    return true;
}

bool Library::returnBook(int id) {
    Book* book = findById(id);
    if (book == nullptr) {
        return false;
    }
    if (book->available) {
        return false;
    }
    book->available = true;
    return true;
}

void Library::listAll() const {
    if (books_.empty()) {
        std::cout << "No books in the library.\n";
        return;
    }

    // Loop over the collection list
    for (const Book& book : books_) {
        std::cout << "  " << book.toString() << "\n";
    }
}

void Library::listAvailable() const {
    bool found = false;
    for (const Book& book : books_) {
        if (book.available) {
            std::cout << "  " << book.toString() << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "No available books.\n";
    }
}

bool Library::saveToFile(const std::string& path) const {
    json root;
    root["nextId"] = nextId_;
    root["books"] = json::array();

    for (const Book& book : books_) {
        json item;
        item["id"] = book.id;
        item["title"] = book.title;
        item["author"] = book.author;
        item["year"] = book.year;
        item["available"] = book.available;
        root["books"].push_back(item);
    }

    std::ofstream out(path);
    if (!out) {
        return false;
    }
    out << root.dump(2);
    return static_cast<bool>(out);
}

bool Library::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return false;
    }

    json root;
    try {
        in >> root;
    } catch (const json::exception&) {
        return false;
    }

    books_.clear();
    nextId_ = root.value("nextId", 1);

    if (!root.contains("books") || !root["books"].is_array()) {
        return true;
    }

    for (const json& item : root["books"]) {
        Book book;
        book.id = item.value("id", 0);
        book.title = item.value("title", "Unknown");
        book.author = item.value("author", "Unknown");
        book.year = item.value("year", 0);
        book.available = item.value("available", true);
        books_.push_back(book);
        if (book.id >= nextId_) {
            nextId_ = book.id + 1;
        }
    }
    return true;
}

int Library::importFromOpenLibraryJson(const std::string& jsonText) {
    std::vector<Book> imported = OpenLibraryApi::parseSearchJson(jsonText);
    int count = 0;
    for (Book& book : imported) {
        book.id = 0;
        addBook(book);
        ++count;
    }
    return count;
}

const std::vector<Book>& Library::getBooks() const {
    return books_;
}

int Library::nextId() const {
    return nextId_;
}

std::size_t Library::size() const {
    return books_.size();
}
