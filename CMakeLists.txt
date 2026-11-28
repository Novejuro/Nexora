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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/pdb")
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY "${OUTPUT_INTERMEDIATE_DIR}/pdb")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# FreeType
add_subdirectory(include/freetype)
if(TARGET freetype AND NOT TARGET Freetype::Freetype)
    add_library(Freetype::Freetype ALIAS freetype)
endif()

# SDL3 and SDL3_image
add_subdirectory(include/sdl)

include(FetchContent)
FetchContent_Declare(SDL3_image
    URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.8/SDL3_image-3.4.8.tar.gz
    URL_HASH SHA256=e8223b424bc7541cf84ffff5cf7e4f24ec3711c60462cb1bd6dc1d7bd7f25477
)
FetchContent_MakeAvailable(SDL3_image)
add_library(SDL_image::SDL_image ALIAS SDL3_image-static)

# RmlUi and its SDL3/OpenGL backend
set(RMLUI_SAMPLES OFF CACHE BOOL "" FORCE)
set(RMLUI_BACKEND "SDL_GL3" CACHE STRING "" FORCE)
set(RMLUI_SDL_VERSION_MAJOR "3" CACHE STRING "" FORCE)
add_subdirectory(include/rmlui)

# The backend expects SDL::SDL to supply both SDL3 and its major version.
add_library(SDL::SDL INTERFACE IMPORTED GLOBAL)
target_link_libraries(SDL::SDL INTERFACE SDL3::SDL3)
target_compile_definitions(SDL::SDL INTERFACE RMLUI_SDL_VERSION_MAJOR=3)
add_subdirectory(include/rmlui/Backends "${CMAKE_BINARY_DIR}/rmlui-backends")

# Slint
add_subdirectory(include/slint)

add_subdirectory(src)
