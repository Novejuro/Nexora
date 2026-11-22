find_package(OpenGL REQUIRED)

if(WIN32)
    set(NEXORA_BACKEND Win32_GL2)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    set(NEXORA_BACKEND X11_GL2)
else()
    message(FATAL_ERROR "The Nexora desktop preview requires Win32 or Linux with X11.")
endif()

if(NOT TARGET rmlui_backend_${NEXORA_BACKEND})
    add_subdirectory("${PROJECT_SOURCE_DIR}/include/rmlui/Backends" rmlui-backends)
endif()

add_executable(Nexora main.cpp LauncherUi.cpp PreviewServices.cpp)
target_link_libraries(Nexora PRIVATE RmlUi::Core rmlui_backend_${NEXORA_BACKEND})
if(WIN32)
    set_target_properties(Nexora PROPERTIES WIN32_EXECUTABLE ON)
    target_link_libraries(Nexora PRIVATE imm32)
else()
    target_link_libraries(Nexora PRIVATE X11::X11)
endif()

set(NEXORA_ASSETS "${PROJECT_SOURCE_DIR}/assets")
set(RMLUI_SAMPLE_ASSETS "${PROJECT_SOURCE_DIR}/include/rmlui/Samples/assets")
set(NEXORA_ASSET_FILES
    "${NEXORA_ASSETS}/launcher.rml"
    "${NEXORA_ASSETS}/launcher.rcss"
    "${NEXORA_ASSETS}/terrain.tga"
    "${RMLUI_SAMPLE_ASSETS}/LatoLatin-Regular.ttf"
    "${RMLUI_SAMPLE_ASSETS}/LatoLatin-Bold.ttf"
)
add_custom_target(nexora_assets
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:Nexora>/assets"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            ${NEXORA_ASSET_FILES} "$<TARGET_FILE_DIR:Nexora>/assets"
    DEPENDS ${NEXORA_ASSET_FILES}
    VERBATIM
)
add_dependencies(Nexora nexora_assets)

install(TARGETS Nexora RUNTIME DESTINATION . COMPONENT Nexora)
install(FILES ${NEXORA_ASSET_FILES} DESTINATION assets COMPONENT Nexora)
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" "${PROJECT_SOURCE_DIR}/README.md"
    DESTINATION . COMPONENT Nexora)
install(FILES "${PROJECT_SOURCE_DIR}/include/rmlui/LICENSE.txt"
    DESTINATION licenses RENAME RmlUi.txt COMPONENT Nexora)
install(FILES "${PROJECT_SOURCE_DIR}/include/freetype/docs/FTL.TXT"
    DESTINATION licenses RENAME FreeType.txt COMPONENT Nexora)
install(FILES "${RMLUI_SAMPLE_ASSETS}/LICENSE.txt"
    DESTINATION licenses RENAME RmlUi-sample-fonts.txt COMPONENT Nexora)
