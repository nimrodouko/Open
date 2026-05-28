#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
struct Vertex {
    glm::vec3 position;
    glm::vec3 Normal;
    glm::vec3 Texturecoods;

};

struct Texture {
    unsigned int id;
    std::string type;
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

