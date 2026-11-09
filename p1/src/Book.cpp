#include "Book.h"
#include <sstream>

Book::Book()
    : id(0), title(""), author(""), year(0), available(true) {}

Book::Book(int id, const std::string& title, const std::string& author, int year, bool available)
    : id(id), title(title), author(author), year(year), available(available) {}

std::string Book::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] \"" << title << "\" by " << author
        << " (" << year << ") - "
        << (available ? "Available" : "Borrowed");
    return oss.str();
}
