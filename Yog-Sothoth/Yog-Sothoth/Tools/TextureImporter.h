#pragma once
#include <string>

struct ImageInfo
{
	int width;
	int height;
	unsigned int bytes;
	std::string name;
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