#include "Library.h"
#include "OpenLibraryApi.h"

#include <iostream>
#include <limits>
#include <string>

namespace {
    const std::string kDataFile = "data/library.json";

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int readInt(const std::string& prompt) {
        int value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearInput();
                return value;
            }
            std::cout << "Please enter a valid number.\n";
            clearInput();
        }
    }

    std::string readLine(const std::string& prompt) {
        std::string line;
        std::cout << prompt;
        std::getline(std::cin, line);
        return line;
    }

    void printMenu() {
        std::cout << "\n========== Library Management System ==========\n"
                  << " 1. List all books\n"
                  << " 2. List available books\n"
                  << " 3. Add a book (manual)\n"
                  << " 4. Search by title\n"
                  << " 5. Search by author\n"
                  << " 6. Borrow a book\n"
                  << " 7. Return a book\n"
                  << " 8. Remove a book\n"
                  << " 9. Search Open Library (internet + JSON)\n"
                  << "10. Save library to file\n"
                  << "11. Load library from file\n"
                  << " 0. Exit\n"
                  << "===============================================\n";
    }

    void addBookManual(Library& library) {
        // Basic variables from user input
        std::string title = readLine("Title: ");
        std::string author = readLine("Author: ");
        int year = readInt("Year: ");

        // Conditional validation
        if (title.empty() || author.empty()) {
            std::cout << "Title and author cannot be empty.\n";
            return;
        }
        if (year < 0 || year > 2100) {
            std::cout << "Year looks invalid.\n";
            return;
        }

        library.addBook(Book(0, title, author, year, true));
        std::cout << "Book added.\n";
    }

    void searchLocal(Library& library, bool byTitle) {
        std::string query = readLine(byTitle ? "Title contains: " : "Author contains: ");
        std::vector<Book> results = byTitle ? library.searchByTitle(query)
                                            : library.searchByAuthor(query);

        if (results.empty()) {
            std::cout << "No matches.\n";
            return;
        }

        for (const Book& book : results) {
            std::cout << "  " << book.toString() << "\n";
        }
    }

    void importFromInternet(Library& library) {
        std::string query = readLine("Search Open Library for: ");
        if (query.empty()) {
            std::cout << "Query cannot be empty.\n";
            return;
        }

        int limit = readInt("How many results to import (1-10)? ");
        if (limit < 1) {
            limit = 1;
        }
        if (limit > 10) {
            limit = 10;
        }

        std::cout << "Fetching from openlibrary.org...\n";
        try {
            std::vector<Book> found = OpenLibraryApi::searchBooks(query, limit);
            if (found.empty()) {
                std::cout << "No results from the API.\n";
                return;
            }

            std::cout << "Found " << found.size() << " book(s):\n";
            for (std::size_t i = 0; i < found.size(); ++i) {
                std::cout << "  " << (i + 1) << ". \"" << found[i].title
                          << "\" by " << found[i].author
                          << " (" << found[i].year << ")\n";
            }

            std::string answer = readLine("Import all into your library? (y/n): ");
            // Conditional on user choice
            if (answer == "y" || answer == "Y") {
                for (Book& book : found) {
                    library.addBook(book);
                }
                std::cout << "Imported " << found.size() << " book(s).\n";
            } else {
                std::cout << "Import cancelled.\n";
            }
        } catch (const std::exception& ex) {
            std::cout << "Internet/JSON error: " << ex.what() << "\n";
        }
    }
}

int main() {
    Library library;

    // File reading on startup (seed data if present)
    if (library.loadFromFile(kDataFile)) {
        std::cout << "Loaded " << library.size() << " book(s) from " << kDataFile << "\n";
    } else {
        std::cout << "No existing library file. Starting empty.\n";
        // Seed a few books so the app is usable offline immediately
        library.addBook(Book(0, "The Hobbit", "J.R.R. Tolkien", 1937));
        library.addBook(Book(0, "1984", "George Orwell", 1949));
        library.addBook(Book(0, "Pride and Prejudice", "Jane Austen", 1813));
        std::cout << "Seeded " << library.size() << " sample book(s).\n";
    }

    bool running = true;

    // Main loop + switch conditionals for menu choices
    while (running) {
        printMenu();
        int choice = readInt("Choose an option: ");

        switch (choice) {
            case 1:
                library.listAll();
                break;
            case 2:
                library.listAvailable();
                break;
            case 3:
                addBookManual(library);
                break;
            case 4:
                searchLocal(library, true);
                break;
            case 5:
                searchLocal(library, false);
                break;
            case 6: {
                int id = readInt("Book ID to borrow: ");
                if (library.borrowBook(id)) {
                    std::cout << "Borrowed successfully.\n";
                } else {
                    std::cout << "Could not borrow (missing or already borrowed).\n";
                }
                break;
            }
            case 7: {
                int id = readInt("Book ID to return: ");
                if (library.returnBook(id)) {
                    std::cout << "Returned successfully.\n";
                } else {
                    std::cout << "Could not return (missing or already available).\n";
                }
                break;
            }
            case 8: {
                int id = readInt("Book ID to remove: ");
                if (library.removeBook(id)) {
                    std::cout << "Removed.\n";
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 9:
                importFromInternet(library);
                break;
            case 10:
                if (library.saveToFile(kDataFile)) {
                    std::cout << "Saved to " << kDataFile << "\n";
                } else {
                    std::cout << "Failed to save file.\n";
                }
                break;
            case 11:
                if (library.loadFromFile(kDataFile)) {
                    std::cout << "Loaded " << library.size() << " book(s).\n";
                } else {
                    std::cout << "Failed to load " << kDataFile << "\n";
                }
                break;
            case 0:
                // Auto-save on exit (file writing)
                if (library.saveToFile(kDataFile)) {
                    std::cout << "Library saved. Goodbye!\n";
                } else {
                    std::cout << "Warning: could not save before exit.\n";
                }
                running = false;
                break;
            default:
                std::cout << "Unknown option. Try again.\n";
                break;
        }
    }

    return 0;
}
