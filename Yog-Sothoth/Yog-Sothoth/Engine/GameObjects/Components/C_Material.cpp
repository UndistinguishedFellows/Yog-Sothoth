#include "C_Material.h"
#include "../../../Tools/TextureImporter.h"

C_Material::~C_Material()
{
}
uint C_Material::LoadTexture(const char* path)
{
	TextureImporter importer;
	texture = importer.LoadTextureBuffer(path);
	return texture;
}
void C_Material::Serialize(Json::Value& root)
{
}

void C_Material::Deserialize(Json::Value& root)
{
}
