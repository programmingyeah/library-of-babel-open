#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct GLFWwindow;
struct Camera;

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void setup();

    void drawFrame(GLFWwindow* window, Camera camera);

private:

    uint32_t shaderProgram = 0;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    uint32_t EBO = 0;
};
