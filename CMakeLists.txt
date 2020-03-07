cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(led-driver LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)

add_library(led-driver SHARED src/display.cpp)
target_link_libraries(led-driver PRIVATE -lpigpio -lpthread)
target_include_directories(led-driver PUBLIC include)
target_compile_options(led-driver PUBLIC -Wall -Wextra -Werror
                                         $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>)

add_subdirectory(test)
