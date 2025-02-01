cmake_minimum_required(VERSION 3.28)
project(TreeAlgorithms LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Function for turning on sanitizers
function(enable_sanitizers target_name)
    target_compile_options(${target_name} PRIVATE -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG)
    target_link_options(${target_name} PRIVATE -fsanitize=address -fsanitize=undefined)
endfunction()


# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(playground)
