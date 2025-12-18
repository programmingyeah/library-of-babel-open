#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



struct GLFWwindow;


struct Camera{
    glm::vec3 pos = {0.0f, 0.0f, 3.0f};
    glm::vec3 front    = {0.0f, 0.0f, -1.0f};
    glm::vec3 up       = {0.0f, 1.0f,  0.0f};

    float fov          = 90.0f;
    float aspectRatio  = 16.0f / 16.0f;
    float nearPlane    = 0.1f;
    float farPlane     = 1000.0f;

    bool firstMouse = true;
    float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0f / 2.0;



    glm::mat4 getViewMatrix() const {
        return glm::lookAt(pos, pos + front, up);
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void setup(GLFWwindow* window);

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void onMouse(double xpos, double ypos);
};

