#pragma once
#include <glm/glm.hpp>
#include "game_object.hpp"

struct GameObject;
class Model;
struct Chunk {
public:
    static inline const glm::vec3 chunkDimensions{ 13.43, 3.1, 10 }; //small buffer space in the y-direction cuz z-clipping

    GameObject object;
    glm::ivec3 gridPos;
    Chunk(Model* model, glm::ivec3 pos)
    : object(model, glm::vec3(pos) * chunkDimensions),
      gridPos(pos)
{}
};