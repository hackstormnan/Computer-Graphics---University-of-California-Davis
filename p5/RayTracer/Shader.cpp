#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

Shader::Shader(const std::string & file_path)
	:m_filePath(file_path), m_id(0)
{
	ShaderProgramSource programSource = ParseShader(file_path);

	m_id = CreateShader(programSource.vertexSource, programSource.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::Bind()const
{
	glUseProgram(m_id);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string  line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	ShaderProgramSource source;
	source.vertexSource = ss[0].str();
	source.fragmentSource = ss[1].str();

	m_filePath = filepath;

	return source;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = GetUniformLocation(name);
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	int location = GetUniformLocation(name);
	glUniform3f(location, v0, v1, v2);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	int location = GetUniformLocation(name);
	glUniform2f(location, v0, v1);
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	int location = GetUniformLocation(name);
	glUniform1f(location, v0);
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
	int location = GetUniformLocation(name);
	glUniform1i(location, v0);
}

void Shader::SetUniformMat4x4(const std::string& name, glm::mat4 mat4)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetUniformVec4(const std::string& name, glm::vec4 vec4)
{
	SetUniform4f(name, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3 vec3)
{
	SetUniform3f(name, vec3.x, vec3.y, vec3.z);
}

void Shader::SetUniformVec2(const std::string& name, glm::vec2 vec2)
{
	SetUniform2f(name, vec2.x, vec2.y);
}

unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	const char* src = source.c_str();

	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		//char message[length];

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;

		std::cout << message;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

int Shader::GetUniformLocation(std::string name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_id, name.c_str());
	if (location == -1) {
		std::cout << "Warnning! " << name << " Uniform doesn't exist!";
	}
	else {
		m_UniformLocationCache[name] = location;
	}

	return location;
}