# Download and link Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
include(CTest)

# Define dependencies
set(TREE_ALGORITHMS tree_algorithms)
set(GTEST_MAIN gtest_main)

# Define target name
set(TEST_TARGET test_tree_algorithms)

# Add test executable
add_executable(${TEST_TARGET}
    ${PROJECT_SOURCE_DIR}/tests/test_tree_algorithms.cpp
)

# TODO: do not duplicate commands for test and benchmark targets
# Enable C++20
target_compile_features(${TEST_TARGET} PUBLIC cxx_std_20)

# Add sanitizers
enable_sanitizers_and_strict_warnings(${TEST_TARGET})


# Link our library and Google Test
target_link_libraries(${TEST_TARGET} PRIVATE ${TREE_ALGORITHMS} ${GTEST_MAIN})


# Enable google test
include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})

add_test(tests ${TEST_TARGET})


target_sources(${TEST_TARGET} PUBLIC
    ${PROJECT_SOURCE_DIR}/tests/components/SimpleAllocator/SimpleAllocator.h
)

# Add Valgrind options
find_program(VALGRIND_EXECUTABLE valgrind)
if (VALGRIND_EXECUTABLE)
    message(STATUS "Valgrind found: ${VALGRIND_EXECUTABLE}")

    set(VALGRIND_COMMAND "${VALGRIND_EXECUTABLE}")
    set(VALGRIND_FLAGS "--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1")

    # Wrap tests with Valgrind
    set_tests_properties(tests
        PROPERTIES
        ENVIRONMENT "GTEST_COLOR=1"
        COMMAND ${VALGRIND_COMMAND} ${VALGRIND_FLAGS} $<TARGET_FILE:${TEST_TARGET}>
    )
else()
    message(WARNING "Valgrind not found; tests and benchmarks will run without Valgrind.")
endif()