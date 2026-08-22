cmake_minimum_required(VERSION 3.10)
project(sgrepxx CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

add_executable(sgrepxx
    src/main.cpp
    src/Config.cpp
    src/Walker.cpp
    src/Matcher.cpp
)

target_include_directories(sgrepxx PRIVATE include)

# std::filesystem needs explicit linking on some older GCC/Clang setups.
target_link_libraries(sgrepxx PRIVATE stdc++fs)
