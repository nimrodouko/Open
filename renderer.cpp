#include <glad/glad.h>
#include "renderer.h"
#include "shader.h"

Mesh::Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<Texture>textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupmesh();
}


 void Mesh::setupmesh() {
	 glGenVertexArrays(1, &VAO);
	 glGenBuffers(1, &VBO);
	 glGenBuffers(1, &EBO);

	 glBindVertexArray(VAO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	
	 glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex),(void *)0);
	 glEnableVertexAttribArray(0);

	 
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	 glEnableVertexAttribArray(1);
	
	 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Texturecoods));
	 glEnableVertexAttribArray(2);

	 glBindVertexArray(0);

}

 void Mesh::Draw(Shader &shader) {

	 unsigned int diffusenr{ 1 };
	 unsigned int specularnr{ 1 };
	 for (unsigned int i{ 0 }; i < textures.size(); i++) {
		 glActiveTexture(GL_TEXTURE0 + i);
		 std::string number;
		 std::string name{ textures[i].type };
		 if (name == "texture_diffuse")
			 number = std::to_string(diffusenr++);
		 else if (name == "texture_specular")
			 number = std::to_string(specularnr++);
		 shader.setint(("material." + name + number).c_str(), i);
		 glBindTexture(GL_TEXTURE_2D, textures[i].id);

	 }
	 glActiveTexture(GL_TEXTURE0);
	 glBindVertexArray(VAO);
	 glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	 glBindVertexArray(0);


 }


 

 void Model::Draw(Shader& shader) {
	 for (unsigned int i{ 0 }; i < meshes.size(); i++) {
		 meshes[i].Draw(shader);
	 }
 }




 void Model::loadmodel(std::string path) {
	 Assimp::Importer import;
	 const aiScene* scene{import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs)};

	 if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		 std::cout << "assimp error" << import.GetErrorString() << std::endl;
		 return;

	 }
	 directory = path.substr(0, path.find_last_of('/'));
	 processNode(scene->mRootNode, scene);
}

 void Model::processNode(aiNode* node, const aiScene* scene) {
	 for (unsigned int i{ 0 }; i < node->mNumMeshes; i++) {
		 aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		 meshes.push_back(processmesh(mesh, scene));
	}
	 for (unsigned int i{ 0 }; i < node->mNumChildren; i++) {
		 processNode(node->mChildren[i], scene);
	 }
 }

 unsigned int TextureFromFile(const char *path, const std::string &directory) {
	 std::string filename{ std::string(path) };
	 filename = directory + "/" + filename;
	 unsigned int textureid;
	 glGenTextures(1, &textureid);
	 int widdth, height, nrchannels;
	 unsigned char* data = stbi_load(filename.c_str(), &widdth, &height, &nrchannels, 0);
	 glBindTexture(GL_TEXTURE_2D, textureid);
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widdth, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	 glGenerateMipmap(GL_TEXTURE_2D);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 stbi_image_free(data);
	 return textureid;



 }


 Model::Model(const char* path)
 {
	loadmodel(path);

 }

 std::vector<Texture>Model::loadmaterialstextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	 std::vector<Texture> textures;
	 for (unsigned int i{ 0 }; i < mat->GetTextureCount(type); i++) {
		 aiString str;
		 mat->GetTexture(type, i, &str);
		 Texture texture;
		 texture.id = TextureFromFile(str.C_Str(), directory);
		 texture.type = typeName;
		 texture.path = str.C_Str();
		 textures.push_back(texture);

	 }
	 return textures;
 }

 Mesh Model::processmesh(aiMesh* mesh, const aiScene* scene) {
	 std::vector <Vertex> vertices;
	 std::vector <unsigned int> indices;
	 std::vector <Texture> textures;

	 for (unsigned int i{ 0 }; i < mesh->mNumVertices; i++) {
		 Vertex vr;
		 glm::vec3 vector;
		 vector.x = mesh->mVertices[i].x;
		 vector.y = mesh->mVertices[i].y;
		 vector.z = mesh->mVertices[i].z;
		 vr.Position = vector;
		 

		 vector.x = mesh->mNormals[i].x;
		 vector.y = mesh->mNormals[i].y;
		 vector.z = mesh->mNormals[i].z;
		 vr.Normal = vector;

		 

		 if (mesh->mTextureCoords[0]) {
			 glm::vec2 vec;
			 vec.x = mesh->mTextureCoords[0][i].x;
			 vec.y = mesh->mTextureCoords[0][i].y;
			 vr.Texturecoods = vec;
		 }
		 else
		 {
			 vr.Texturecoods = glm::vec2(0.0f, 0.0f);
		 }
		 vertices.push_back(vr);

	 }
	 for (unsigned int i{ 0 }; i < mesh->mNumFaces; i++) { //kumbuka to reduce the computations in this process.
		 aiFace face = mesh->mFaces[i];
		 for (unsigned int j{ 0 }; j < face.mNumIndices; j++) {
			 indices.push_back(face.mIndices[j]);
		 }
	 }

	 if (mesh->mMaterialIndex >= 0) {
		 aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		 std::vector<Texture> diffusemaps = loadmaterialstextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		 textures.insert(textures.end(), diffusemaps.begin(), diffusemaps.end());
		 std::vector<Texture> specularmaps = loadmaterialstextures(material, aiTextureType_SPECULAR, "texture_specular");
		 textures.insert(textures.end(), specularmaps.begin(), specularmaps.end());



	 }

	 return Mesh(vertices, indices, textures);
 

 }






























