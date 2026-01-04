#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <chrono>
#include <memory>
#include "camera.hpp"
#include "renderer.hpp"
#include "game_object.hpp"
#include "mesh.hpp"

float vertices[] = {
    // positions         // colors
   -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f
};

unsigned int indices[] = {
    0,1,2, 2,3,0, // back face
    4,5,6, 6,7,4, // front face
    0,1,5, 5,4,0, // bottom
    2,3,7, 7,6,2, // top
    0,3,7, 7,4,0, // left
    1,2,6, 6,5,1  // right
};

static std::string loadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR::FILE_NOT_FOUND: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Renderer::setup() {
    // load shader source code

    std::string vertCode = loadFile("src/shaders/vert.glsl");
    std::string fragCode = loadFile("src/shaders/frag.glsl");

    const char* vertSrc = vertCode.c_str();
    const char* fragSrc = fragCode.c_str();

    // create shader objects
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSrc, nullptr);
    glCompileShader(vertShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
        std::cout << "ERROR::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSrc, nullptr);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        std::cout << "ERROR::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link programs
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
}



void Renderer::drawBatches(std::unordered_map<Model*, std::vector<glm::mat4>> batches){
    for (auto& [model, transforms] : batches) {
        for (Mesh& mesh : model->meshes) {
            //once per mesh

            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;

            for (unsigned int j = 0; j < mesh.textures.size(); j++) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, mesh.textures[j].id);
                
                std::string number;
                if (mesh.textures[j].type == "texture_diffuse") number = std::to_string(diffuseNr++);
                else if (mesh.textures[j].type == "texture_specular") number = std::to_string(specularNr++);
                
                // lazy cache uniform location per shader
                GLuint loc;
                if (mesh.textures[j].cachedLoc.count(shaderProgram) == 0) {
                    loc = glGetUniformLocation(shaderProgram, ("material." + mesh.textures[j].type + number).c_str());
                    mesh.textures[j].cachedLoc[shaderProgram] = loc;
                } else {
                    loc = mesh.textures[j].cachedLoc[shaderProgram];
                }

                glUniform1i(loc, j);
            }

            glActiveTexture(GL_TEXTURE0);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVBO);
            glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_DYNAMIC_DRAW);
            glBindVertexArray(mesh.VAO);
            glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, transforms.size());
            
        }
    }
}

void Renderer::drawFrame(GLFWwindow* window, Camera camera, std::vector<GameObject*> objects) {
    int SCR_WIDTH = 0;
    int SCR_HEIGHT = 0;
    glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::unordered_map<Model*, std::vector<glm::mat4>> batches;
    for (GameObject* obj : objects) {
        glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), obj->position);
        batches[obj->model].push_back(modelMat);
    }

    glUseProgram(shaderProgram);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    GLuint locViewPos = glGetUniformLocation(shaderProgram, "uViewPos");
    glUniform3fv(locViewPos, 1, glm::value_ptr(camera.pos));


    auto start = std::chrono::high_resolution_clock::now();

    drawBatches(batches);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "updateChunksAndTransforms took " << duration << " μs\n";
}