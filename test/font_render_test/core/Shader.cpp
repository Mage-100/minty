#include <iostream>
#include <string>
#include <fstream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "Shader.hpp"

static std::string ShaderError(const std::string& msg) {
	return "[ShaderError] " + msg;
}

static void checkShaderCompilation(unsigned int shader, std::string msg) {
	int success;
	char infoLog[512];

	glad_glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glad_glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		throw std::runtime_error("ERROR::SHADER::" + msg + "::COMPILATION_FAILED\n" + infoLog);
	}
}

static void checkShaderProgramCompilation(unsigned int shaderProgram, std::string msg) {
	int success;
	char infoLog[512];

	glad_glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glad_glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		throw std::runtime_error("ERROR::SHADER_PROGRAM::" + msg + "::COMPILATION_FAILED\n" + infoLog);
	}
}

Shader::Shader(const std::string& shaderPath) {
	try {
		std::fstream shaderCode(shaderPath, std::ios::in);
		if (!shaderCode.is_open())
			throw std::runtime_error("Failed to open file: " + shaderPath);

		std::string line;

		int state = -1;
		
		while (!shaderCode.eof()) {
			std::getline(shaderCode, line);

			if (line == "#shader(vertex)") {
				state = 0;
				continue;
			}
			else if (line == "#shader(fragment)") {
				state = 1;
				continue;
			}

			if (state == 0) {
				_vertexShaderSource.append(line);
				_vertexShaderSource.append(1, '\n');
			}
			else if (state == 1) {
				_fragmentShaderSource.append(line);
				_fragmentShaderSource.append(1, '\n');
			}
		}
		shaderCode.close();

		_compileVertexShader();
		_compileFragmentShader();
		_compileShaderProgram();

	}
	catch (std::exception& e) {
		std::cerr << ShaderError(e.what()) << std::endl;
	}
}

Shader::~Shader() {
	glad_glDeleteShader(_vertexShader);
	glad_glDeleteShader(_fragmentShader);

}

void Shader::_compileVertexShader() {
	_vertexShader = glad_glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderCode = _vertexShaderSource.c_str();
	glad_glShaderSource(_vertexShader, 1, &vertexShaderCode, nullptr);
	glad_glCompileShader(_vertexShader);
	checkShaderCompilation(_vertexShader, "VERTEX");
}

void Shader::_compileFragmentShader() {
	_fragmentShader = glad_glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderCode = _fragmentShaderSource.c_str();
	glad_glShaderSource(_fragmentShader, 1, &fragmentShaderCode, nullptr);
	glad_glCompileShader(_fragmentShader);
	checkShaderCompilation(_fragmentShader, "FRAGMENT");

}

void Shader::_compileShaderProgram() {
	_shaderProgram = glad_glCreateProgram();
	glad_glAttachShader(_shaderProgram, _vertexShader);
	glad_glAttachShader(_shaderProgram, _fragmentShader);
	glad_glLinkProgram(_shaderProgram);
	checkShaderProgramCompilation(_shaderProgram, "shader_program");
}

int Shader::_getUniformLocation(const std::string& name) {
	int location = glad_glGetUniformLocation(_shaderProgram, name.c_str());

	if (location == -1) {
		std::cerr << "Could not find uniform: " << name << std::endl;
		throw std::runtime_error("Could not find uniform");
	}

	return location;
}

void Shader::use() {
	glad_glUseProgram(_shaderProgram);
}

void Shader::setFloat(const std::string& name, GLfloat v0) {
	int location = _getUniformLocation(name);
	glad_glUniform1f(location, v0);
}

void Shader::setFloat2(const std::string& name, GLfloat v0, GLfloat v1) {
	int location = _getUniformLocation(name);
	glad_glUniform2f(location, v0, v1);
}

void Shader::setFloat3(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2) {
	int location = _getUniformLocation(name);
	glad_glUniform3f(location, v0, v1, v2);
}

void Shader::setFloat4(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	int location = _getUniformLocation(name);
	glad_glUniform4f(location, v0, v1, v2, v3);
}

void Shader::setMat4(const std::string& name, glm::mat4& m) {
	int location = _getUniformLocation(name);
	glad_glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}
