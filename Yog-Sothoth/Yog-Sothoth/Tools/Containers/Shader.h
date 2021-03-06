﻿#pragma once
#include <glew.h>
#include <string>
#include "../../MathGeoLib/Math/float2.h"

class Shader
{
public:
	unsigned int ID;
	std::string vertexPath;
	std::string fragmentPath;
	std::string geometryPath;

	Shader(const char* _vertexPath,
			const char* _fragmentPath,
			const char* _geometryPath = nullptr);
	~Shader();

	void LoadShader();

	// activate the shader
	// ------------------------------------------------------------------------
	void Use();
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const;
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const float2* value) const;
	void setVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const float3 *value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const float4 *value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const float2x2 *mat) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const float3x3 *mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const float4x4 *mat) const;


private:
	void checkCompileErrors(GLuint shader, std::string type);
};
