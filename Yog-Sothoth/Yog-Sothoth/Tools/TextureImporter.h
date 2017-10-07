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
	unsigned int loadTexture(const char* path);
};