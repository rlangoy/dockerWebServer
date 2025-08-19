cmake_minimum_required(VERSION 3.15)
project(cpp_webserver)

set(CMAKE_CXX_STANDARD 17)

# Conan will generate config files inside build/
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_package(fmt REQUIRED)

add_executable(webserver src/main.cpp)
target_link_libraries(webserver PRIVATE fmt::fmt)
