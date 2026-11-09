#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include <string>
#include <vector>

// Collection list: std::vector stores all books in memory
class Library {
public:
    Library();

    void addBook(const Book& book);
    bool removeBook(int id);
    Book* findById(int id);
    std::vector<Book> searchByTitle(const std::string& query) const;
    std::vector<Book> searchByAuthor(const std::string& query) const;

    bool borrowBook(int id);
    bool returnBook(int id);

    void listAll() const;
    void listAvailable() const;

    // File reading / writing (JSON on disk)
    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);

    // Import books parsed from Open Library JSON text
    int importFromOpenLibraryJson(const std::string& jsonText);

    const std::vector<Book>& getBooks() const;
    int nextId() const;
    std::size_t size() const;

private:
    std::vector<Book> books_;
    int nextId_;
};

#endif
