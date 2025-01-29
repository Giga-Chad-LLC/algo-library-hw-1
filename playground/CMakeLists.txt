# Add test executable
set(PLAYGROUND_TARGET playground_tree_algorithms)

add_executable(${PLAYGROUND_TARGET}
    ${PROJECT_SOURCE_DIR}/playground/main.cpp
)

# Enable C++20
target_compile_features(${PLAYGROUND_TARGET} PUBLIC cxx_std_20)

# Link our library
target_link_libraries(${PLAYGROUND_TARGET} PRIVATE tree_algorithms)
