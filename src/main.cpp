#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include "renderer.hpp"
#include "camera.hpp"
#include "mesh.hpp"

float deltaTime, lastFrame;

void processInput(GLFWwindow* window, float deltaTime, Camera &camera);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

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
    renderer.setup();    


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    camera.setup(window);
    renderer.setup();  

    std::string path = "/home/bakedsteak/projects/library-of-babel-opengl/assets/room1.obj";
    std::cout << "Loading models..." << std::endl;
    Model model(path);
    std::cout << "Models loaded" << std::endl;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int height, width;

        glfwGetWindowSize(window, &width, &height);
        camera.aspectRatio = (float) width/ (float) height;

        processInput(window, deltaTime, camera);
        renderer.drawFrame(window, camera, model);

        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaTime, Camera &camera){

    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 15.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * (camera.front - glm::dot(camera.front, UP) * UP);  //project to xzplane.
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * (camera.front - glm::dot(camera.front, UP) * UP);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.pos += UP * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        camera.pos -= UP * cameraSpeed;

}
