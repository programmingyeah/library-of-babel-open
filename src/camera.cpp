#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include "renderer.hpp"
#include "camera.hpp"






/*void processInput(GLFWwindow *window, float deltaTime){

    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 1.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * (cameraFront - glm::dot(cameraFront, Up) * Up);  //project to xzplane.
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * (cameraFront - glm::dot(cameraFront, Up) * Up);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += Up * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        cameraPos -= Up * cameraSpeed;

}*/

void Camera::setup(GLFWwindow* window){
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, this->mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* cam = static_cast<Camera*>(
        glfwGetWindowUserPointer(window)
    );

    if (!cam) return;

    //cam->onMouse(xpos, ypos);


    if (cam->firstMouse)
    {
        cam->lastX = xpos;
        cam->lastY = ypos;
        cam->firstMouse = false;
    }
  
    float xoffset = xpos - cam->lastX;
    float yoffset = cam->lastY - ypos; 
    cam->lastX = xpos;
    cam->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->yaw   += xoffset;
    cam->pitch += yoffset;

    if(cam->pitch > 89.0f)
        cam->pitch = 89.0f;
    if(cam->pitch < -89.0f)
        cam->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    direction.y = sin(glm::radians(cam->pitch));
    direction.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    cam->front = glm::normalize(direction);
} 


