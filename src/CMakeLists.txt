
# # Require Ninja as the generator
# if (NOT CMAKE_GENERATOR STREQUAL "Ninja")
#     message(FATAL_ERROR "This project requires Ninja as the generator. Please re-run CMake with: cmake -G Ninja")
# endif()


add_library(tree_algorithms STATIC)

# Add source files
target_sources(tree_algorithms PUBLIC
    ${PROJECT_SOURCE_DIR}/src/impl/tree_algorithms.cpp
)
# Add source modules
target_sources(tree_algorithms
    PUBLIC
    FILE_SET CXX_MODULES FILES
        ${PROJECT_SOURCE_DIR}/src/modules/tree_algorithms.cpp
)

# Enable C++20 modules
target_compile_features(tree_algorithms PUBLIC cxx_std_20)

# Include directories (for modules)
target_include_directories(tree_algorithms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

