cmake_minimum_required(VERSION 3.12)
project(GamEncin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Source Files ---
file(GLOB_RECURSE PROJECT_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src/Encin/Items/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src/Encin/Items/Components/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src/Encin/Tools/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src/User/*.cpp"
)

set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLAD/glad.c")
set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)

set(ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/Resources/resources.rc")

# --- Source Files ---
add_executable(GamEncin
    ${PROJECT_SOURCE}
    ${GLAD_SOURCE}
    ${ICON_RESOURCE}
)

# --- Include Directories ---
target_include_directories(GamEncin PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies"
)

# --- Target Specific Library Directories ---
link_directories(GamEncin PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW"
)

# --- Platform Specifics for Windows ---
if(WIN32 AND MSVC)
    set_target_properties(GamEncin PROPERTIES
        WIN32_EXECUTABLE $<$<NOT:$<CONFIG:Debug>>:TRUE>
    )
endif()


# --- Link Libraries ---
target_link_libraries(GamEncin PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/glfw3.lib"
    opengl32
    gdi32
    user32
    shell32
)

# --- Output Directory (Optional) ---
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# --- Copy Resources ---
set(PROJECT_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/Resources")
if(EXISTS ${PROJECT_RESOURCE_DIR})
    add_custom_command(TARGET GamEncin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${PROJECT_RESOURCE_DIR}"
        "$<TARGET_FILE_DIR:GamEncin>/Resources"
        COMMENT "Copying resources to build directory"
    )
else()
    message(WARNING "Project resource directory not found: ${PROJECT_RESOURCE_DIR}")
endif()