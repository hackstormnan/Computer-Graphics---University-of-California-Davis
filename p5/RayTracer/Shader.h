#pragma once
#include <string>
#include <unordered_map>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string & file_path);
	~Shader();

	void Bind()const;
	void UnBind();

	//Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4x4(const std::string& name, glm::mat4 mat4);
	void SetUniformVec4(const std::string& name, glm::vec4 vec4);
	void SetUniformVec3(const std::string& name, glm::vec3 vec3);
	void SetUniformVec2(const std::string& name, glm::vec2 vec2);

	std::string m_filePath;
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	ShaderProgramSource ParseShader(const std::string& filepath);

	int GetUniformLocation(std::string name);
};
