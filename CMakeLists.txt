cmake_minimum_required(VERSION 3.20)
project(WaterAdventure C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)



# GLFW
FetchContent_Declare(
        GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(GLFW)

# GLM
FetchContent_Declare(
        GLM
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
)
FetchContent_MakeAvailable(GLM)

#STB Image
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)

#IMGUI
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.89.9
)
FetchContent_MakeAvailable(imgui)

# FAST GLTF
FetchContent_Declare(
        fastgltf
        GIT_REPOSITORY https://github.com/spnda/fastgltf
        GIT_TAG main
)
FetchContent_MakeAvailable(fastgltf)

# FREETYPE
FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2
)

# Disable Unity Build for FreeType to avoid conflicts
FetchContent_GetProperties(freetype)
if(NOT freetype_POPULATED)
    FetchContent_Populate(freetype)
    set(CMAKE_UNITY_BUILD_BACKUP ${CMAKE_UNITY_BUILD})
    set(CMAKE_UNITY_BUILD OFF)
    add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
    set(CMAKE_UNITY_BUILD ${CMAKE_UNITY_BUILD_BACKUP})
endif()



add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)


# Disable Unity Build for imgui to avoid OpenGL symbol conflicts
set_target_properties(imgui PROPERTIES UNITY_BUILD OFF)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

add_library(glad_c STATIC Deps/glad/src/glad.c
        include/glmDebugger.h
        include/FastNoise.h)
target_include_directories(glad_c PUBLIC Deps/glad/include)


# Gather all source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(WaterAdventure ${SRC_FILES}
        Deps/stb/src/stb_image_impl.cpp
        include/Renderer/Window.h
        include/Core/Camera.h
        include/App.h
        include/GameObject.h
        include/Renderer/Renderer.h
        include/Core/Scene.h
        include/Core/UiManager.h
)

set_target_properties(WaterAdventure PROPERTIES
        LINKER_LANGUAGE CXX
)

target_include_directories(WaterAdventure PRIVATE
        include
        src
        ${GLFW_SOURCE_DIR}/include
        ${GLM_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${stb_SOURCE_DIR}
        ${fastgltf_SOURCE_DIR}/include
        ${freetype_SOURCE_DIR}/include
        ${libigl_SOURCE_DIR}/include

)

#PCH
target_precompile_headers(WaterAdventure PRIVATE
        <vector>
        <string>
        <unordered_map>
        <glm/glm.hpp>
)
target_link_libraries(WaterAdventure PRIVATE glad_c)

target_link_libraries(WaterAdventure PRIVATE glfw glm::glm  imgui fastgltf freetype )

