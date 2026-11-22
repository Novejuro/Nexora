cmake_minimum_required(VERSION 4.0)

project(Nexora
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OUTPUT_BASE_DIR "${PROJECT_SOURCE_DIR}/build")
set(OUTPUT_INTERMEDIATE_DIR "${OUTPUT_BASE_DIR}/obj")

if("${CMAKE_BINARY_DIR}" STREQUAL "${OUTPUT_BASE_DIR}")
    message(FATAL_ERROR
        "Configure Nexora with -B build/obj so CMake-generated files stay out of build/."
    )
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/$<0:>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/$<0:>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/lib/$<0:>")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/pdb")
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/pdb")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(include/freetype)

if(TARGET freetype AND NOT TARGET Freetype::Freetype)
    add_library(Freetype::Freetype ALIAS freetype)
endif()
# FreeType

add_subdirectory(include/sdl)
# SDL

set(RMLUI_SAMPLES OFF CACHE BOOL "" FORCE)
set(RMLUI_BACKEND "SDL_VK" CACHE STRING "" FORCE)
set(RMLUI_SDL_VERSION_MAJOR "3" CACHE STRING "" FORCE)
add_subdirectory(include/rmlui)
# RmlUi

add_library(SDL::SDL INTERFACE IMPORTED GLOBAL)
target_link_libraries(SDL::SDL INTERFACE SDL3::SDL3)
target_compile_definitions(SDL::SDL INTERFACE RMLUI_SDL_VERSION_MAJOR=3)

add_subdirectory(include/rmlui/Backends "${CMAKE_BINARY_DIR}/rmlui-backends")

add_subdirectory(src)
