#include <glad/glad.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Shader::Shader(const char* vertexpath, const char* fragmentpath) {
	std::string vertexcode;
	std::string fragmentcode;
	std::ifstream vshaderfile;
	std::ifstream fshaderfile;

	vshaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fshaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vshaderfile.open(vertexpath);
		fshaderfile.open(fragmentpath);
		std::stringstream vshaderstream, fshaderstream;
		vshaderstream << vshaderfile.rdbuf();
		fshaderstream << fshaderfile.rdbuf();
		vshaderfile.close();
		fshaderfile.close();

		vertexcode = vshaderstream.str();
		fragmentcode = fshaderstream.str();

	}
	catch(std::ifstream::failure e){
		std::cout << "shader file not succesfully rad" << "\n";
	}
	const char* vshadercode{ vertexcode.c_str() };
	const char* fshadercode{ fragmentcode.c_str()};
	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex, 1, &vshadercode, NULL);
	glShaderSource(fragment, 1 ,& fshadercode, NULL);

	glCompileShader(vertex);
	glCompileShader(fragment);
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id,fragment);
	glLinkProgram(id);

	glDeleteShader(vertex);
	glDeleteShader(fragment);


}
void Shader::use() {

	glUseProgram(id);

}

void Shader::setbool(const std::string& name, bool value) {

	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);

}

void Shader::setint(const std::string& name, int value) {

	glUniform1i(glGetUniformLocation(id, name.c_str()), value);

}

void Shader::setfloat(const std::string& name, float value) {

	glUniform1f(glGetUniformLocation(id, name.c_str()), value);

}

void Shader::setmatrixuniform(const std::string& name, glm::mat4 &value) {

	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),1, GL_FALSE, glm::value_ptr(value));

}
void Shader::setvec3(const std::string& name,glm::vec3 &value) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}