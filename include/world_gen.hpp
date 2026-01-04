#include <glm/glm.hpp>
#include <memory>
#include "camera.hpp"
#include "mesh.hpp"
#include "chunk.hpp"

struct World {
    World(const std::string& modelPath)
    : model(modelPath) {}

    Model model;
    void worldGen(std::vector<GameObject*>& objects, Camera camera);
};