cmake_minimum_required(VERSION 4.0)

project(Nexora
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OUTPUT_BASE_DIR "${PROJECT_SOURCE_DIR}/build")
set(OUTPUT_INTERMEDIATE_DIR "${CMAKE_BINARY_DIR}")

if("${CMAKE_BINARY_DIR}" STREQUAL "${OUTPUT_BASE_DIR}")
    message(FATAL_ERROR
        "Configure Nexora with -B build/obj so CMake-generated files stay out of build/."
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/$<$<CONFIG:Debug>:debug>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/$<$<CONFIG:Debug>:debug>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/lib/$<0:>")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/pdb")
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/pdb")


set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(include/freetype)

if(TARGET freetype AND NOT TARGET Freetype::Freetype)
    add_library(Freetype::Freetype ALIAS freetype)
endif()

add_subdirectory(include/rmlui)
add_subdirectory(src)
