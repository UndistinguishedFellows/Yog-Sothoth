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
	//root["resource_uuid"] = rMaterial->uuid;
	root["checkers"] = checkers;

	root["ambient"].append(ambient.x);
	root["ambient"].append(ambient.y);
	root["ambient"].append(ambient.z);

	root["diffuse"].append(ambient.x);
	root["diffuse"].append(ambient.y);
	root["diffuse"].append(ambient.z);

	root["shininess"] = shininess;

	root["color"].append(color.r);
	root["color"].append(color.g);
	root["color"].append(color.b);
	root["color"].append(color.a);
}

void C_Material::Deserialize(Json::Value& root)
{
}
