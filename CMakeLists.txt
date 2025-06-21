cmake_minimum_required(VERSION 3.25)
project(GamEncin LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# --- Source Files ---
file(GLOB_RECURSE PROJECT_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src/**/*.cpp"
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
#if(WIN32 AND MSVC)
#    set_target_properties(GamEncin PROPERTIES
#        WIN32_EXECUTABLE $<$<NOT:$<CONFIG:Debug>>:TRUE>
#    )
#endif()

# --- Link Libraries ---
target_link_libraries(GamEncin PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/src/libglfw3.a" # Replace with MinGW-compatible library
    opengl32
    gdi32
    user32
    shell32
)

# --- Output Directory ---
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