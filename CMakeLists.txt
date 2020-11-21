cmake_minimum_required(VERSION 3.11)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

project (IsoMaze)

# Find libs
find_library(ALLEGRO_LIBRARY NAMES liballeg44.dll.a)

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable (IsoMaze ${SOURCES} ${HEADERS})

target_compile_options(IsoMaze PRIVATE -Wall -Wextra -pedantic)

# Link Libs
target_link_libraries(IsoMaze ${ALLEGRO_LIBRARY})