# file(GLOB_RECURSE PROJECT_FILES CONFIGURE_DEPENDS
#     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
# ) # Automatically Scan Files

set(TARGET_NAME Nexora)

add_executable(${TARGET_NAME}
    main.cpp
    app/App.cpp
    core/MessageBox/MessageBox.cpp
    # ${PROJECT_FILES} # Add Scanned Files
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        RmlUi::RmlUi
        rmlui_backend_SDL_GL3
        Slint::Slint
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Fontconfig REQUIRED)
    target_link_libraries(slint_cpp INTERFACE Fontconfig::Fontconfig)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(OpenGL REQUIRED)
    target_link_libraries(slint_cpp INTERFACE OpenGL::GL)
endif()

slint_target_sources(${TARGET_NAME}
    "${PROJECT_SOURCE_DIR}/assets/ui"
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
)

if(WIN32)
    set_target_properties(${TARGET_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME "Nexora"
)
