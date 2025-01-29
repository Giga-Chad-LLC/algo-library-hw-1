cmake_minimum_required(VERSION 3.28)
project(TreeAlgorithms LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(playground)


# # LIB
# add_library(tree_algorithms STATIC)

# # Add source files
# target_sources(tree_algorithms PUBLIC
#         ${PROJECT_SOURCE_DIR}/src/impl/tree_algorithms.cpp
# )
# target_sources(tree_algorithms
#     PUBLIC
#         FILE_SET CXX_MODULES
#         FILES
#             ${PROJECT_SOURCE_DIR}/src/modules/tree_algorithms.cpp
# )

# # Enable C++20 modules
# target_compile_features(tree_algorithms PUBLIC cxx_std_20)

# # Include directories (for modules)
# target_include_directories(tree_algorithms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})





# # TESTS
# # Download and link Google Test
# include(FetchContent)

# FetchContent_Declare(
#     googletest
#     URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
# )
# FetchContent_MakeAvailable(googletest)

# # Add test executable
# add_executable(test_tree_algorithms
#     ${PROJECT_SOURCE_DIR}/tests/test_tree_algorithms.cpp
# )

# target_compile_features(test_tree_algorithms PUBLIC cxx_std_20)

# target_link_libraries(test_tree_algorithms PRIVATE tree_algorithms gtest_main)

# # Enable testing
# include(GoogleTest)
# gtest_discover_tests(test_tree_algorithms)