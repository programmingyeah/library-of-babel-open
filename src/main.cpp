#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include "renderer.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "chunk.hpp"
#include <algorithm>

int GEN_RADIUS = 8;

float deltaTime, lastFrame;
std::vector<Chunk> chunks{};

void processInput(GLFWwindow* window, float deltaTime, Camera &camera);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::filesystem::path getExecutableDir() {
    char buf[4096];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        return std::filesystem::path(buf).parent_path();
    }
    throw std::runtime_error("Cannot determine executable path");
}

glm::ivec3 gridPosFromWorldPos(glm::vec3 pos) {
    return glm::ivec3(glm::floor(pos / Chunk::chunkDimensions));
}

Chunk generateChunk(glm::ivec3 gridPos, Model* model) {
    //will be expanded on later
    Chunk chunk(model, gridPos);

    return chunk;
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Library of Babel", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;
    }

    Camera camera{};
    Renderer renderer{};
    renderer.setup();    

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    camera.setup(window);
    renderer.setup();  

    std::filesystem::path exeDir = getExecutableDir();
    std::filesystem::path assetPath =
    exeDir.parent_path() / "assets" / "room" / "finished6.obj";

    std::string path = assetPath.string();
    std::cout << "Loading models..." << std::endl;
    Model model(path);
    std::cout << "Models loaded" << std::endl;

    std::vector<GameObject*> objects;

    glm::ivec3 prevPlrGridPos = glm::ivec3(10000,100000,100);//to check if player has entered a new chunk
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int height, width;

        glfwGetWindowSize(window, &width, &height);
        camera.aspectRatio = (float) width/ (float) height;

        processInput(window, deltaTime, camera);

        // chunk generation
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

            // extract object data
            objects.clear();
            for (Chunk& chunk : chunks) {
                objects.push_back(&chunk.object);
            }

            prevPlrGridPos = plrGridPos;
        }

        
        renderer.drawFrame(window, camera, objects);

        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaTime, Camera &camera){

    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * glm::normalize(camera.front - glm::dot(camera.front, UP) * UP);  //project to xzplane.
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * glm::normalize(camera.front - glm::dot(camera.front, UP) * UP);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.pos += UP * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        camera.pos -= UP * cameraSpeed;

}
