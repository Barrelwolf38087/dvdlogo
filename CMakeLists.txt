cmake_minimum_required(VERSION 3.17)
project(dvdlogo)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(dvdlogo src/main.cpp src/logo.hpp)

target_link_libraries(dvdlogo sfml-system sfml-window sfml-graphics)
