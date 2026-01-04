#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>


struct GLFWwindow;
struct Camera;
struct GameObject;
class Model;

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void setup();

    void drawFrame(GLFWwindow* window, Camera camera, std::vector<GameObject*> objects);

    void drawBatches(std::unordered_map<Model*, std::vector<glm::mat4>> batches);

    void drawModel(Model* model);

private:
    uint32_t shaderProgram = 0;
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    uint32_t EBO = 0;
};
