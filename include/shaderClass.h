#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class Shader {
public:
	GLuint ID;
	Shader(const std::string& vertexFile, const std::string& fragmentFile);

	void Activate();
	void Delete();
	/* utility uniform functions */
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, glm::mat4 mat);
	void setVec3(const std::string& name, glm::vec3 vec);

private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif // !SHADER_CLASS_H
