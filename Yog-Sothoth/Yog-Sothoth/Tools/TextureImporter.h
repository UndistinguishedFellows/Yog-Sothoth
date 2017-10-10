#pragma once
#include "il.h"
#include "ilu.h"
#include "ilut.h"

struct ImageInfo
{
	int width;
	int height;
	unsigned int bytes;
};

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	//GLuint loadTextureTuto(const char* path);
	unsigned int LoadTexture(const char* path);
	unsigned int LoadTextureBuffer(const char* path, ImageInfo *im_info = nullptr);
};