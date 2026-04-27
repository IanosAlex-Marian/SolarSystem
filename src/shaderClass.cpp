#include "shaderClass.h"

/* Get a string from a text file*/
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);

	if (!in.is_open())
	{
		std::cout << "ERROR: Could not open file: " << filename << std::endl;
		return "";
	}

	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();

	return contents;
}

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile.c_str());
	std::string fragmentCode = get_file_contents(fragmentFile.c_str());

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	/* Create vertex shader object */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	/* Attach source to object */
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	/* Compile */
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	/* Create fragment shader object */
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	/* Attach source to object */
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	/* Compile */
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	/* Create shader program object */
	ID = glCreateProgram();

	/* Attach shaders to program */
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	/* Link shaders */
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	/* Delete shaders for safe memory */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 mat)
{
	glUniformMatrix4fv(
		glGetUniformLocation(ID, name.c_str()),
		1,
		GL_FALSE,
		glm::value_ptr(mat)
	);
}

/* Error handling */
void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (std::string(type) != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << '\n' <<  infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << '\n' << infoLog << std::endl;
		}
	}
}