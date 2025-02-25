# Add test executable
set(PLAYGROUND_TARGET playground_tree_algorithms)

add_executable(${PLAYGROUND_TARGET}
    ${PROJECT_SOURCE_DIR}/playground/main.cpp
    # scenarios
    ${PROJECT_SOURCE_DIR}/playground/scenarios/Random/Random.cpp
    ${PROJECT_SOURCE_DIR}/playground/scenarios/Scenario/Scenario.h

    # components
    ${PROJECT_SOURCE_DIR}/playground/components/Report/Report.cpp
    ${PROJECT_SOURCE_DIR}/playground/components/ExecutionTimer/ExecutionTimer.cpp
)

# Enable C++20
target_compile_features(${PLAYGROUND_TARGET} PUBLIC cxx_std_20)

# Add sanitizers
enable_sanitizers_and_strict_warnings(${PLAYGROUND_TARGET})

# Link our library
target_link_libraries(${PLAYGROUND_TARGET} PRIVATE tree_algorithms)


# TODO: duplicates the Valgrind code from tests/CMakeLists.txt
# Conditionally use Valgrind
 find_program(VALGRIND_EXECUTABLE valgrind)
 if (VALGRIND_EXECUTABLE)
     message(STATUS "Valgrind found: ${VALGRIND_EXECUTABLE}")

     # Add valgrind execution flags
     set(VALGRIND_FLAGS "--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1")

     # Automatically run playground target with Valgrind
     add_custom_command(
         TARGET ${PLAYGROUND_TARGET}
         POST_BUILD
         COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_FLAGS} $<TARGET_FILE:${PLAYGROUND_TARGET}>
         COMMENT "Running playground with Valgrind..."
     )
 else()
     message(WARNING "Valgrind not found; playground will run without Valgrind.")
 endif()