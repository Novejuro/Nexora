# file(GLOB_RECURSE PROJECT_FILES CONFIGURE_DEPENDS
#     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
# ) # Automatically Scan Files

set(TARGET_NAME Nexora)

add_executable(${TARGET_NAME}
    main.cpp
    app/App.hpp
    app/App.cpp
    # ${PROJECT_FILES} # Add Scanned Files
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        RmlUi::RmlUi
        rmlui_backend_SDL_VK
)

if(WIN32)
    set_target_properties(${TARGET_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME "Nexora"
)
