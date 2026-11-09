#ifndef BOOK_H
#define BOOK_H

#include <string>

// Basic variables wrapped in a Book type (id, title, author, year, available)
struct Book {
    int id;
    std::string title;
    std::string author;
    int year;
    bool available;

    Book();
    Book(int id, const std::string& title, const std::string& author, int year, bool available = true);

    std::string toString() const;
};

#endif
