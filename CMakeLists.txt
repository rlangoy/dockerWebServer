cmake_minimum_required(VERSION 3.12)
project(webserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)

add_executable(webserver src/main.cpp)

target_link_libraries(webserver boost::boost spdlog::spdlog)


