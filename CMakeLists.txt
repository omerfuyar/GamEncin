cmake_minimum_required(VERSION 3.10)
project(MyGameEngine)

# Add all source files
set(SOURCES
    GamEncin/src/Engine.cpp
    GamEncin/src/main.cpp
)

# Add the executable
add_executable(MyGameEngine ${SOURCES})

# Include the header files in the src/ directory
target_include_directories(MyGameEngine PUBLIC src)