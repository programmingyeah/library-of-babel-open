#include "camera.hpp"
#include "input.hpp"
#include <GLFW/glfw3.h>

float deltaTime, lastFrame;

void processInput(GLFWwindow *window, float currentFrame, Camera &camera){
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * glm::normalize(camera.front - glm::dot(camera.front, UP) * UP);  //project to xzplane.
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * glm::normalize(camera.front - glm::dot(camera.front, UP) * UP);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.pos += UP * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        camera.pos -= UP * cameraSpeed;

}
