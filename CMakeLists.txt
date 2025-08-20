cmake_minimum_required(VERSION 3.12)
project(webserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost (Beast needs system for networking)
find_package(Boost REQUIRED COMPONENTS system)

# Find fmt
find_package(fmt REQUIRED)

add_executable(webserver src/main.cpp)

target_link_libraries(webserver
    PRIVATE Boost::system
    PRIVATE fmt::fmt
)
