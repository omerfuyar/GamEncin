# GamEncin

## IMPORTANT

This project is one of my old projects, developed over 5 months. I learned so much from this project, and I really respect it, even though I find it disgusting when I look back at it. This project is discontinued until further notice. Notice to the huge void who is reading this... (🤠) The last thing I did was configure the build system and update the "How to use" section in this file. But who knows, maybe I can come back and work on this project again.

## Description

GamEncin is a simple game engine exclusive to Windows written in C++ using OpenGL for rendering. It was created as a personal project to explore game engine architecture. The design is heavily inspired by the Unity engine, featuring a Scene/Object/Component model.

## Features

-   **Unity-Inspired Architecture**: The engine is built around a `Scene` that holds multiple `Object` instances. Functionality is added to objects using `Component`s, similar to Unity's `MonoBehaviour` system.
-   **Scene Management**: You can create and manage multiple scenes. Scenes are created and connected by writing C++ code, as there is no graphical editor.
-   **Object Lifecycle**: Objects in a scene follow a defined lifecycle with methods like `Awake()`, `Start()`, and `Update()`, which are called automatically by the engine's main loop.
-   **Core Components**: Every object has fundamental properties like position, rotation, and scale (Transform component). The engine also provides built-in components for rendering (`ModelMesh`), physics (`RigidBody` and `Collider`) etc.

## Project Structure

The project is organized into the following main directories:

-   `Dependencies/`: Contains third-party libraries like GLAD, GLFW, and GLM. they are inside the repository, so you don't need to download them separately.
-   `GamEncin/src/Encin/`: The core engine source code, including the main application loop, scene/object management, rendering tools and built in components.
-   `GamEncin/src/User/`: Example code demonstrating how to use the engine, including user scenes and custom components.
-   `CMakeLists.txt`: The build configuration file for the project.

## The Game Controls
-  **WASD**: Move around.
-  **E**: Interact.
-  **F**: Attack.

## How to use

The project uses CMake for building. You will need to have CMake and MSVC (Visual Studio) installed on your system.

1.  Create a `build` folder and navigate to it.
2.  ```powershell
    mkdir build
    cd build
    ```
3.  Run the following command to generate the build files:
    ```powershell
    cmake ..
    ```
4.  Then, run the build command:
    ```powershell
    cmake --build . --config Release
    ```
5.  The executable will be created in the `build/bin/Release` directory.

## Dependencies

-   [GLFW](https://www.glfw.org/)
-   [GLAD](https://glad.dav1d.de/)
-   [GLM](https://glm.g-truc.net/0.9.9/index.html)
-   Note: Some of these libraries might have been slightly modified for the project.

## Archive

Here is the link for the project's Google Drive folder for historical fun footage:
[Google Drive](https://drive.google.com/drive/folders/1W-77e2p9_lwJLx5HURER91UriWdcW32x?usp=sharing)
