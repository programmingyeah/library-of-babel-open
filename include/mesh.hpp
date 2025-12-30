#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <assimp/scene.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        unsigned int VAO, VBO, EBO;
    private:

        void setupMesh();
};

class Model 
{
    public:
        Model(std::string path)
        {
            loadModel(path);
        }

        std::vector<Mesh> meshes;
    private:
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             std::string typeName);
};