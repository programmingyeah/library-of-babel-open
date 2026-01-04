cmake_minimum_required(VERSION 3.20)
project(LibraryOfBabel LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Copy shaders
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders
     DESTINATION ${CMAKE_BINARY_DIR}/src)

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW
add_subdirectory(external/glfw)

# Assimp
add_subdirectory(external/assimp)

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# STB (header-only)
add_library(stb INTERFACE)
target_include_directories(stb SYSTEM INTERFACE external/stb)

# ============================
# Add all your project sources explicitly
# ============================
set(SRC
    src/main.cpp
    src/render/renderer.cpp
    src/render/mesh.cpp
    src/input.cpp
    src/world/game_object.cpp
    src/world/world_gen.cpp
    src/camera.cpp
)

add_executable(LoB ${SRC})

target_include_directories(LoB PRIVATE include src)

target_link_libraries(LoB PRIVATE
    glad
    glfw
    assimp
    glm
    stb
)

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(LoB PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
elseif(UNIX)
    target_link_libraries(LoB PRIVATE
        dl
        X11
        pthread
        GL
    )
endif()
