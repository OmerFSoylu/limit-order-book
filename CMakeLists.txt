cmake_minimum_required(VERSION 3.16)
project(lob LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lob
    main.cpp
    OrderBook.cpp
)

target_compile_options(lob PRIVATE
    -Wall -Wextra -Wpedantic
)