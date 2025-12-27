#pragma once
#include <cstdint>
#include <glm/glm.hpp>

class Model;

struct GameObject {
    Model* model;
    glm::vec3 position;

    GameObject(Model* mod, glm::vec3 pos)
        : model(mod), position(pos) {}

    void Draw(unsigned int shaderProgram);
};
