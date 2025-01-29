# Download and link Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(test_tree_algorithms
    ${PROJECT_SOURCE_DIR}/tests/test_tree_algorithms.cpp
)

# Enable C++20
target_compile_features(tree_algorithms PUBLIC cxx_std_20)

# Link our library and Google Test
target_link_libraries(test_tree_algorithms PRIVATE tree_algorithms gtest_main)

# Enable testing
include(GoogleTest)
gtest_discover_tests(test_tree_algorithms)