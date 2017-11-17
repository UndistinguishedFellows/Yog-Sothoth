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

	root["diffuse"].append(diffuse.x);
	root["diffuse"].append(diffuse.y);
	root["diffuse"].append(diffuse.z);

	root["shininess"] = shininess;

	root["color"].append(color.r);
	root["color"].append(color.g);
	root["color"].append(color.b);
	root["color"].append(color.a);

	root["type"] = type;
}

void C_Material::Deserialize(Json::Value& root)
{
	type = (ComponentType)root.get("type", 0).asInt();
	checkers = root.get("chechers", false).asBool();

	shininess = root.get("shininess", 0.5).asFloat();

	Json::Value col = root["color"];
	Json::Value::iterator it = col.begin();
	color.r = (*it++).asFloat();
	color.g = (*it++).asFloat();
	color.g = (*it++).asFloat();
	color.a = (*it).asFloat();

	col = root["ambient"];
	it = col.begin();
	ambient.x = (*it++).asFloat();
	ambient.y = (*it++).asFloat();
	ambient.z = (*it++).asFloat();

	col = root["diffuse"];
	it = col.begin();
	diffuse.x = (*it++).asFloat();
	diffuse.y = (*it++).asFloat();
	diffuse.z = (*it++).asFloat();
}
