#pragma once

enum ShaderType
{
	VERTEX,
	FRAGMENT
};

class Shader
{
public:
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	Shader();
	~Shader();

	void LoadShader(char* path, ShaderType type);
	void CompileProgram(unsigned int vertexShader, unsigned int fragmentShader);
	void Use();
};
