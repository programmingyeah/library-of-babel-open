cmake_minimum_required(VERSION 3.20)
project(LibraryOfBabel)

set(CMAKE_CXX_STANDARD 17)

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders
     DESTINATION ${CMAKE_BINARY_DIR}/src) # copy shaders into build to ensure proper relative dir 

include_directories(external/glad/include)
include_directories(external/glfw/include)
include_directories(include)
include_directories(src)

add_library(glad external/glad/src/glad.c)

add_subdirectory(external/glfw)

file(GLOB SRC "src/*.cpp" "src/**/*.cpp")
add_executable(LoB ${SRC})

target_link_libraries(LoB PRIVATE glad glfw)

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(LoB PRIVATE ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(UNIX)
    target_link_libraries(LoB PRIVATE dl X11 pthread GL)
endif()
