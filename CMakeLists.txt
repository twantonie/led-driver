cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(led-driver LANGUAGES CXX C)

add_library(led-driver SHARED src/display.cpp)
target_link_libraries(led-driver PRIVATE -lpigpio -lpthread)
target_include_directories(led-driver PUBLIC include)

add_subdirectory(test)
