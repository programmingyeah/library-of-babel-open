#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>
#include <memory>
#include "game_object.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "world_gen.hpp"

int GEN_RADIUS = 8;
glm::ivec3 prevPlrGridPos = glm::ivec3(10000,100000,100);//to check if player has entered a new chunk
std::vector<Chunk> chunks{};


glm::ivec3 gridPosFromWorldPos(glm::vec3 pos) {
    return glm::ivec3(glm::floor(pos / Chunk::chunkDimensions));
}

Chunk generateChunk(glm::ivec3 gridPos, Model* model) {
    //will be expanded on later
    Chunk chunk(model, gridPos);

    return chunk;
}

void World::worldGen(std::vector<GameObject*>& objects, Camera camera){
    //Chunk generation
    glm::ivec3 plrGridPos = gridPosFromWorldPos(camera.pos);

    if (plrGridPos != prevPlrGridPos) {
        //add chunks
        for (int dx = -GEN_RADIUS; dx <= GEN_RADIUS; dx++) 
        for (int dy = -GEN_RADIUS; dy <= GEN_RADIUS; dy++) 
        {
            //note we only care about a 2d slice 
            glm::ivec3 targetPos = plrGridPos+glm::ivec3(dx,dy,0);
            targetPos.z = 0;

            bool foundChunk = false;
            for (const Chunk& chunk : chunks) {
                if (chunk.gridPos == targetPos) {
                    foundChunk = true;
                    continue;
                }
            }
            if (foundChunk == true) continue;

            //generate chunk if it doesnt exist
            chunks.push_back(generateChunk(targetPos, &model));
        }

        //remove chunks
        for (int i = chunks.size() - 1; i >= 0; --i) {
            glm::ivec3 diff = abs(chunks[i].gridPos - plrGridPos);
            int maxDist = std::max({diff.x, diff.y, diff.z}); 


            if (maxDist > GEN_RADIUS) {
                chunks.erase(chunks.begin() + i);
            }
        }


        
        //extract objects
  
        objects.clear();
        for (Chunk& chunk : chunks) {
            //GameObject* obj = new GameObject(chunk.object);
            //objects->push_back(obj);
            objects.push_back(&chunk.object);

        }
        std::cout<<objects.size()<<std::endl;

        prevPlrGridPos = plrGridPos; 
    }
}