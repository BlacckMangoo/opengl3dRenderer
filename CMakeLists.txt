cmake_minimum_required(VERSION 3.10)
project(WaterAdventure)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
        GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG master
)
FetchContent_MakeAvailable(GLFW)

# GLM
FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG master
)
FetchContent_MakeAvailable(GLM)

#STB Image
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)

#ASSIMP - Optimized build (disable unnecessary features)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp
        GIT_TAG master
)

FetchContent_MakeAvailable(assimp)

# Gather all source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(WaterAdventure ${SRC_FILES}
        Deps/glad/src/glad.c
        Deps/stb/src/stb_image_impl.cpp
        include/Renderer/Window.h
        src/Window.cpp
        src/Core/Camera.cpp
        include/Core/Camera.h
        include/App.h
        src/App.cpp
        src/Renderer/Mesh.cpp
        include/GameObject.h
        include/Renderer/Renderer.h
        include/Core/Scene.h
        src/Renderer/Model.cpp
)

target_include_directories(WaterAdventure PRIVATE
        include
        src
        Deps/glad/include
        ${stb_SOURCE_DIR}
)

target_link_libraries(WaterAdventure PRIVATE glfw glm assimp)
