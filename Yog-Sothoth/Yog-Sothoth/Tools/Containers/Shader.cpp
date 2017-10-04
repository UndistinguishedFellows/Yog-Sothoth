#include "Shader.h"
#include "../Yog-Sothoth/Engine/CoreModules/M_FileSystem.h"
#include "../Yog-Sothoth/Application.h"
#include <glew.h>


Shader::Shader() : vertexShader(0), fragmentShader(0), shaderProgram(0)
{
	
}

Shader::~Shader()
{
}

void Shader::LoadShader(char* path, ShaderType type)
{
	char* buffer;
	int success;
	int size = App->fs->load(path, &buffer);
	buffer[size-1] = '\0';
	switch (type)
	{
		case VERTEX_SHADER:
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &buffer, nullptr);
			glCompileShader(vertexShader);			
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{	
				char infoLog[512];
				glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
				yogConsole(CONSOLE_ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
			}
			break;
		case FRAGMENT_SHADER:
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &buffer, nullptr);
			glCompileShader(fragmentShader);
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
				yogConsole(CONSOLE_ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
			}

			break;case GEOMETRY_SHADER:
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &buffer, nullptr);
			glCompileShader(geometryShader);
			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char infoLog[512];
				glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
				yogConsole(CONSOLE_ERROR, "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n%s\n", infoLog);
			}

			break;
	}


	delete buffer;
}

void Shader::CompileProgram()
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		yogConsole(CONSOLE_ERROR, "ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() const
{
	glUseProgram(shaderProgram);
}
