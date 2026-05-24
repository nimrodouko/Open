#include <glad/glad.h>
#include "renderer.h"
#include "shader.h"



 void Mesh::setupmesh() {
	 glGenVertexArrays(1, &VAO);
	 glGenBuffers(1, &VBO);
	 glGenBuffers(1, &EBO);

	 glBindVertexArray(VAO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);



}


Mesh::Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<Texture>textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupmesh();
}
