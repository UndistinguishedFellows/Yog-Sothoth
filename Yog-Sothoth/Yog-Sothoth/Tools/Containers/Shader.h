#pragma once

enum ShaderType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER
};

class Shader
{
public:
	unsigned int vertexShader = 0;
	unsigned int fragmentShader = 0;
	unsigned int geometryShader = 0;
	unsigned int shaderProgram = 0;

	Shader();
	~Shader();

	void LoadShader(char* path, ShaderType type);
	void CompileProgram();
	void Use() const;
};
