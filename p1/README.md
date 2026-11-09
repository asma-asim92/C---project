# Library Management System

A simple computer program that helps you manage books — like a tiny digital library on your PC.

You can add books, look them up, borrow and return them, save your list, and even find new books online.

---

## What can it do?

- See all your books
- Add a new book yourself
- Search by title or author
- Borrow or return a book
- Delete a book
- Find books on the internet and add them to your list
- Save your library so it is still there next time

---

## How to start it

### Step 1 — Install a C++ tool (one time only)

This program is written in C++. Your computer needs a free “compiler” to turn the code into an app.

Pick **one**:

- [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) (choose “Desktop development with C++”), or
- [MSYS2](https://www.msys2.org/) (then install MinGW), or
- [LLVM](https://releases.llvm.org/) (clang)

Ask a classmate or teacher if you are not sure which one to use.

### Step 2 — Build the app

1. Open the project folder (`p1`)
2. Double-click **`build.bat`**, or open a terminal in that folder and type:

```
build.bat
```

3. When it finishes, go into the **`build`** folder
4. Run **`library_ms.exe`**

---

## How to use the menu

When the program starts, you will see a numbered menu. Type a number and press Enter.

| Number | What it does |
|--------|----------------|
| 1 | Show all books |
| 2 | Show only books you can borrow |
| 3 | Add a book by typing the details |
| 4 | Search by title |
| 5 | Search by author |
| 6 | Borrow a book (use the book’s ID number) |
| 7 | Return a book |
| 8 | Remove a book |
| 9 | Search for books online and import them |
| 10 | Save your library to a file |
| 11 | Load your library from a file |
| 0 | Exit (it saves automatically) |

**Tip:** Option **9** needs an internet connection.

---

## Where is my data saved?

Your books are stored in a file called:

`data/library.json`

You do not need to edit this file yourself. The program reads and writes it for you.

---

## Quick try-out

1. Press **1** to see the starter books  
2. Press **6**, then enter a book ID (for example `1`) to borrow it  
3. Press **9**, type a search like `harry potter`, and import the results  
4. Press **0** to quit — your list is saved  

---

## Need help?

If `build.bat` says **“No C++ compiler found”**, the free tool from Step 1 is not installed yet (or the terminal cannot see it). Install it, restart the terminal, and try again.
