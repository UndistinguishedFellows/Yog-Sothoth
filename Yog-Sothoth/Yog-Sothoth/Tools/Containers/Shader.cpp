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
		case VERTEX:
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
		case FRAGMENT:
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

			break;
	}


	delete buffer;
}

void Shader::CompileProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
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
