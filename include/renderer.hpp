#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct GLFWwindow;

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void setup();

    void drawFrame(GLFWwindow* window, glm::vec3 cameraPos,  glm::vec3 cameraFront, glm::vec3 cameraUp);

private:

    uint32_t shaderProgram = 0;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    uint32_t EBO = 0;
};
