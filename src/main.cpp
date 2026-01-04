#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <algorithm>
#include <memory>
#include "renderer.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "chunk.hpp"
#include "input.hpp"
#include "world_gen.hpp"



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::filesystem::path getExecutableDir() {
    char buf[4096];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        return std::filesystem::path(buf).parent_path();
    }
    throw std::runtime_error("Cannot determine executable path");
} 

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Library of Babel", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;
    }

    Camera camera{};
    Renderer renderer{};

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    camera.setup(window);
    renderer.setup();  

    std::filesystem::path exeDir = getExecutableDir();
    std::filesystem::path assetPath =
    exeDir.parent_path() / "assets" / "room" / "finished6.obj";

    std::string path = assetPath.string();
    std::cout << "Loading models..." << std::endl;    
    World world(path);
    std::cout << "Models loaded" << std::endl;

    std::vector<GameObject*> objects;
    //std::vector<std::unique_ptr<GameObject>> objects;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();    

        int height, width;

        glfwGetWindowSize(window, &width, &height);
        camera.aspectRatio = (float) width/ (float) height;

        processInput(window, glfwGetTime(), camera);

        // chunk generation
        world.worldGen(objects, camera);


        renderer.drawFrame(window, camera, objects);



        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}
