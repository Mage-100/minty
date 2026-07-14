#pragma once
#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

// A basic shader that reads the shader files together

class Shader {
	unsigned int _vertexShader;
	unsigned int _fragmentShader;
	unsigned int _shaderProgram;

	std::string _vertexShaderSource;
	std::string _fragmentShaderSource;
public:
	Shader(const std::string& shaderPath);
	~Shader();

	void use();

	void setFloat(const std::string& name, GLfloat v0);
	void setFloat2(const std::string& name, GLfloat v0, GLfloat v1);
	void setFloat3(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
	void setFloat4(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

	template<typename T>
	void setVec2(const std::string& name, glm::vec2 v);
	template<typename T>
	void setVec3(const std::string& name, glm::vec3 v);
	template<typename T>
	void setVec4(const std::string& name, glm::vec4 v);

	template<>
	void setVec2<float>(const std::string& name, glm::vec2 v);
	template<>
	void setVec3<float>(const std::string& name, glm::vec3 v);
	template<>
	void setVec4<float>(const std::string& name, glm::vec4 v);


	void setMat2(const std::string& name, glm::mat2 m);
	void setMat3(const std::string& name, glm::mat3 m);
	void setMat4(const std::string& name, glm::mat4 m);
private:
	void _compileVertexShader();
	void _compileFragmentShader();
	void _compileShaderProgram();

	int _getUniformLocation(const std::string& name);
};