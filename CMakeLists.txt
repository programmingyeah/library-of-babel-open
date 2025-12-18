cmake_minimum_required(VERSION 3.20)
project(LibraryOfBabel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------
# Assets
# ------------------------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders
     DESTINATION ${CMAKE_BINARY_DIR}/src)

# ------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# GLFW
add_subdirectory(external/glfw)

# Assimp
add_subdirectory(external/assimp)

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

# stb (header-only)
add_library(stb INTERFACE)
target_include_directories(stb SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
)

# ------------------------------------------------------------
# Executable
# ------------------------------------------------------------

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(LoB ${SRC})

target_include_directories(LoB PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(LoB PRIVATE
    glad
    glfw
    assimp
    glm
    stb
)

# ------------------------------------------------------------
# Platform-specific OpenGL
# ------------------------------------------------------------

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
