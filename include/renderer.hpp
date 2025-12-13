#pragma once
#include <cstdint>

struct GLFWwindow;

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void setup();

    void drawFrame(GLFWwindow* window);

private:

    uint32_t shaderProgram = 0;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    uint32_t EBO = 0;
};
