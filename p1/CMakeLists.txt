cmake_minimum_required(VERSION 3.16)
project(LibraryManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(library_ms
    src/main.cpp
    src/Book.cpp
    src/Library.cpp
    src/HttpClient.cpp
    src/OpenLibraryApi.cpp
)

target_include_directories(library_ms PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# External module: WinHTTP (Windows system library)
if(WIN32)
    target_link_libraries(library_ms PRIVATE winhttp)
endif()

# Copy seed JSON next to the binary so relative path data/library.json works
add_custom_command(TARGET library_ms POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:library_ms>/data
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/data/library.json
            $<TARGET_FILE_DIR:library_ms>/data/library.json
)
