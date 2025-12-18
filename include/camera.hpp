#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern glm::vec3 Up;

extern float deltaTime;	// time between current frame and last frame
extern float lastFrame;

extern bool firstMouse;
extern float yaw;	
extern float pitch;
extern float lastX;
extern float lastY;




class Camera{

public:
    Camera() = default;
    ~Camera() = default;

    
    void processInput(GLFWwindow* window, float deltaTime);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

private:
 


};