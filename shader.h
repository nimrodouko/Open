#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Shader {
public:
	unsigned int id;
	Shader(const char* vertexpath, const char* fragmentpath);
	void use();
	void setbool(const std::string& name, bool value);
	void setint(const std::string& name, int value);
	void setfloat(const std::string& name, float value);
	void setmatrixuniform(const std::string& name, glm::mat4 &value);
	void setvec3(const std::string& name, glm::vec3 &value) const;
};