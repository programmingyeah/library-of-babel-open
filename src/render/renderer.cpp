#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
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

void Renderer::drawModel(Model* model) {
    if (!model) return;

    for(unsigned int i = 0; i < model->meshes.size(); i++) {
        glUseProgram(shaderProgram); 

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for(unsigned int j = 0; j < model->meshes[i].textures.size(); j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);

            std::string number;
            std::string name = model->meshes[i].textures[j].type;

            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            if (model->meshes[i].textures[j].id != 0)
                glBindTexture(GL_TEXTURE_2D, model->meshes[i].textures[j].id);

            glUniform1i(
                glGetUniformLocation(shaderProgram, ("material." + name + number).c_str()),
                j
            );
        }

        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(model->meshes[i].VAO);
        glDrawElements(GL_TRIANGLES, model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Renderer::drawFrame(GLFWwindow* window, Camera camera, std::vector<GameObject*> objects) {
    int SCR_WIDTH = 0;
    int SCR_HEIGHT = 0;

    glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw commands
    /*for (GameObject* obj : objects) {

        glm::mat4 model = glm::translate(glm::mat4(1.0), obj->position);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        glUseProgram(shaderProgram);

        //uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
        GLuint loc = glGetUniformLocation(shaderProgram, "uViewPos");
        glUniform3fv(loc, 1, glm::value_ptr(camera.pos));

        drawModel(obj->model);
    }*/

    std::unordered_map<Model*, std::vector<glm::mat4>> batches;

    for (GameObject* obj : objects) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), obj->position);
        batches[obj->model].push_back(model);
    }


    for (auto& [model, transforms] : batches) {

        for (Mesh& mesh : model->meshes) {
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix();

            glUseProgram(shaderProgram);

            //bind textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;

            for(unsigned int j = 0; j < mesh.textures.size(); j++)
            {
                glActiveTexture(GL_TEXTURE0 + j);

                std::string number;
                std::string name = mesh.textures[j].type;

                if(name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = std::to_string(specularNr++);

                if (mesh.textures[j].id != 0)
                    glBindTexture(GL_TEXTURE_2D, mesh.textures[j].id);

                glUniform1i(
                    glGetUniformLocation(shaderProgram, ("material." + name + number).c_str()),
                    j
                );
            }

            glActiveTexture(GL_TEXTURE0);

            //uniforms
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
            GLuint loc = glGetUniformLocation(shaderProgram, "uViewPos");
            glUniform3fv(loc, 1, glm::value_ptr(camera.pos));

            //bind and initialize instancing data
            glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVBO);
            glBufferData(
                GL_ARRAY_BUFFER,
                transforms.size() * sizeof(glm::mat4),
                transforms.data(),
                GL_DYNAMIC_DRAW
            );

            //bind VAO and draw call
            glBindVertexArray(mesh.VAO);
            glDrawElementsInstanced(
                GL_TRIANGLES,
                mesh.indices.size(),
                GL_UNSIGNED_INT,
                0,
                transforms.size()
            );
        }
    }
}