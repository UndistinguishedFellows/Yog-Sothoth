#include "Shader.h"
#include "../Yog-Sothoth/Engine/CoreModules/M_FileSystem.h"
#include "../Yog-Sothoth/Application.h"
#include <glew.h>


Shader::Shader(const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	vertexPath = _vertexPath;
	fragmentPath = _fragmentPath;
	if (_geometryPath != nullptr)
	{
		geometryPath = _geometryPath;		
	}

	LoadShader();
}

Shader::~Shader()
{
}

void Shader::LoadShader()
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	char* vShaderFile;
	char* fShaderFile;
	char* gShaderFile;
	int vShaderFile_size;
	int fShaderFile_size;
	int gShaderFile_size;
	bool loadingError = false;
	try
	{
		// open files
		vShaderFile_size = App->fs->Load(vertexPath.c_str(), &vShaderFile);
		fShaderFile_size = App->fs->Load(fragmentPath.c_str(), &fShaderFile);
		if (true)
			int i;
		else
			int a;

		if (vShaderFile_size == 0)
		{
			yogConsole(CONSOLE_ERROR, "Error loading %s", vertexPath);
			loadingError = true;
		}
		else
		{
			vShaderFile[vShaderFile_size - 1] = '\0';
		}
		if (fShaderFile_size == 0)
		{
			yogConsole(CONSOLE_ERROR, "Error loading %s", fragmentPath);
			loadingError = true;
		}
		else
		{
			fShaderFile[fShaderFile_size - 1] = '\0';
		}

		// if geometry shader path is present, also load a geometry shader
		if (geometryPath.size() > 0)
		{
			gShaderFile_size = App->fs->Load(geometryPath.c_str(), &gShaderFile);
			if (gShaderFile_size == 0)
			{
				yogConsole(CONSOLE_ERROR, "Error loading %s", geometryPath);
				loadingError = true;
			}
			else
			{
				gShaderFile[gShaderFile_size - 1] = '\0';
			}
			if (loadingError)
			{
				return;
			}
			geometryCode = gShaderFile;
		}
		if (loadingError)
		{
			return;
		}
		vertexCode = vShaderFile;
		fragmentCode = fShaderFile;

	}
	catch (std::ifstream::failure e)
	{
		yogConsole(CONSOLE_ERROR, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometryPath.size() > 0)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath.size() > 0)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath.size() > 0)
		glDeleteShader(geometry);
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::Use()
{
	glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const float2* value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, (const GLfloat*)value);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const float3 *value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, (const GLfloat*)value);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const float4 *value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, (const GLfloat*)value);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const float2x2* mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, (const GLfloat*)mat);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const float3x3 *mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, (const GLfloat*)mat);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const float4x4 *mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, (const GLfloat*)mat);
}




void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			yogConsole(CONSOLE_ERROR, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-- --------------------------------------------------- --", type.c_str(), infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			yogConsole(CONSOLE_ERROR, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n-- --------------------------------------------------- --", type.c_str(), infoLog);
		}
	}
}
