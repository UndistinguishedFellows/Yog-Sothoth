#include "C_Light.h"

C_Light::~C_Light()
{
}

void C_Light::Serialize(Json::Value& root)
{
	root["ambient"].append(ambient.x);
	root["ambient"].append(ambient.y);
	root["ambient"].append(ambient.z);

	root["diffuse"].append(ambient.x);
	root["diffuse"].append(ambient.y);
	root["diffuse"].append(ambient.z);

	root["specular"].append(ambient.x);
	root["specular"].append(ambient.y);
	root["specular"].append(ambient.z);

	root["type"] = type;
}

void C_Light::Deserialize(Json::Value& root)
{
}
