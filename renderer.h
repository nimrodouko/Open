#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "shader.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Texturecoods;

};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<Texture>textures);
    void Draw(Shader& shader);
private:
    unsigned int VAO, VBO, EBO;
    void setupmesh();
};

class Model {
public: 
    Model(const char* path);
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    void loadmodel(std::string path);
    Mesh processmesh(aiMesh* mesh, const aiScene* scene);
    void processNode(aiNode* node, const aiScene* scene);
    std::vector<Texture>loadmaterialstextures(aiMaterial* mat, aiTextureType type, std::string typeName);


};


