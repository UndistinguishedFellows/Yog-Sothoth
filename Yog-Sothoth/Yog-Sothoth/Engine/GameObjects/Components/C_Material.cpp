#include "C_Material.h"
#include "../../../Tools/TextureImporter.h"
#include <glew.h>

C_Material::~C_Material()
{
}
uint C_Material::LoadTexture(const char* path)
{
	TextureImporter importer;
	if (texture != 0)
	{
		glDeleteTextures(1, &texture);
	}
	texture = importer.LoadTextureBuffer(path, &imInfo);
	return texture;
}
void C_Material::Serialize(Json::Value& root)
{
}

void C_Material::Deserialize(Json::Value& root)
{
}
