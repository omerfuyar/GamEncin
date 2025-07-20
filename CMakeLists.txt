cmake_minimum_required(VERSION 3.25)
project(GamEncin LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- MSVC-Specific Settings for Windows ---
if(MSVC)
    message(STATUS "Configuring for MSVC (Visual Studio)")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else()
    message(FATAL_ERROR "This project is configured for Windows and MSVC only.")
endif()

# --- Dependencies (from local repository) ---

# GLAD (compile C source and include headers)
set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLAD/glad.c")
set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLAD")

# GLFW (Import pre-compiled library)
add_library(glfw_precompiled STATIC IMPORTED)
set_target_properties(glfw_precompiled PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/src/libglfw3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW"
)

# GLM (header-only library)
set(GLM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/")

# --- Main Project Source Files ---
file(GLOB_RECURSE PROJECT_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src/**/*.cpp"
)
set(ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/Resources/resources.rc")

# --- Executable Target ---
add_executable(GamEncin
    ${PROJECT_SOURCE}
    ${GLAD_SOURCE}
    ${ICON_RESOURCE}
)

# --- Include Directories ---
target_include_directories(GamEncin PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/GamEncin/src"
    ${GLAD_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# --- Link Libraries ---
target_link_libraries(GamEncin PRIVATE
    glfw_precompiled
    opengl32
    gdi32
    user32
    shell32
)

# --- Fix for MSVC Linker Errors with MinGW-compiled GLFW ---
if(MSVC)
    target_link_libraries(GamEncin PRIVATE legacy_stdio_definitions)
endif()

# --- Copy Resources to Build Directory ---
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
