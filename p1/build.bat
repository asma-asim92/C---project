@echo off
setlocal

REM Build with MSVC (Developer Command Prompt) OR MinGW g++
REM Usage: build.bat

if not exist build mkdir build

where cl >nul 2>&1
if %ERRORLEVEL%==0 (
    echo Building with MSVC...
    cl /EHsc /std:c++17 /I include /I external /Fe:build\library_ms.exe ^
        src\main.cpp src\Book.cpp src\Library.cpp src\HttpClient.cpp src\OpenLibraryApi.cpp ^
        /link winhttp.lib
    if errorlevel 1 exit /b 1
    if not exist build\data mkdir build\data
    copy /Y data\library.json build\data\library.json >nul
    echo.
    echo Built: build\library_ms.exe
    echo Run:   cd build ^&^& library_ms.exe
    exit /b 0
)

where g++ >nul 2>&1
if %ERRORLEVEL%==0 (
    echo Building with g++...
    g++ -std=c++17 -I include -I external -o build\library_ms.exe ^
        src\main.cpp src\Book.cpp src\Library.cpp src\HttpClient.cpp src\OpenLibraryApi.cpp ^
        -lwinhttp
    if errorlevel 1 exit /b 1
    if not exist build\data mkdir build\data
    copy /Y data\library.json build\data\library.json >nul
    echo.
    echo Built: build\library_ms.exe
    echo Run:   cd build ^&^& library_ms.exe
    exit /b 0
)

where clang++ >nul 2>&1
if %ERRORLEVEL%==0 (
    echo Building with clang++...
    clang++ -std=c++17 -I include -I external -o build\library_ms.exe ^
        src\main.cpp src\Book.cpp src\Library.cpp src\HttpClient.cpp src\OpenLibraryApi.cpp ^
        -lwinhttp
    if errorlevel 1 exit /b 1
    if not exist build\data mkdir build\data
    copy /Y data\library.json build\data\library.json >nul
    echo.
    echo Built: build\library_ms.exe
    echo Run:   cd build ^&^& library_ms.exe
    exit /b 0
)

echo No C++ compiler found.
echo Install one of:
echo   - Visual Studio Build Tools (MSVC)
echo   - MSYS2 / MinGW-w64 (g++)
echo   - LLVM (clang++)
echo Then re-run build.bat from a terminal where the compiler is on PATH.
exit /b 1
