cmake_minimum_required(VERSION 3.28)
project(TreeAlgorithms LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(playground)
