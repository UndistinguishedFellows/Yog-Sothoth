#pragma once
#include "il.h"
#include "ilu.h"
#include "ilut.h"

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	//GLuint loadTextureTuto(const char* path);
	unsigned int LoadTexture(const char* path);
	unsigned int LoadTextureBuffer(const char* path);
};