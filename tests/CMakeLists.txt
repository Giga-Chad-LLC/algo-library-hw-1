# Download and link Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

# Define target name
set(TEST_TARGET test_tree_algorithms)

# Add test executable
add_executable(${TEST_TARGET}
    ${PROJECT_SOURCE_DIR}/tests/test_tree_algorithms.cpp
)

# Enable C++20
target_compile_features(${TEST_TARGET} PUBLIC cxx_std_20)


# Add sanitizers
enable_sanitizers_and_strict_warnings(${TEST_TARGET})


# TODO: use the set variable instead of hardcoding the library name
# Link our library and Google Test
target_link_libraries(${TEST_TARGET} PRIVATE tree_algorithms gtest_main)

# Enable testing
include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})