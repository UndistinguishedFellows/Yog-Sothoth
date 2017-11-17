#include "C_Light.h"

C_Light::~C_Light()
{
}

void C_Light::Serialize(Json::Value& root)
{
	root["ambient"].append(ambient.x);
	root["ambient"].append(ambient.y);
	root["ambient"].append(ambient.z);

	root["diffuse"].append(diffuse.x);
	root["diffuse"].append(diffuse.y);
	root["diffuse"].append(diffuse.z);

	root["specular"].append(specular.x);
	root["specular"].append(specular.y);
	root["specular"].append(specular.z);

	root["type"] = type;
}

void C_Light::Deserialize(Json::Value& root)
{
	type = (ComponentType)root.get("type", 0).asInt();

	Json::Value col = root["color"];
	Json::Value::iterator it = col.begin();
	specular.x = (*it++).asFloat();
	specular.y = (*it++).asFloat();
	specular.z = (*it).asFloat();

	col = root["ambient"];
	it = col.begin();
	ambient.x = (*it++).asFloat();
	ambient.y = (*it++).asFloat();
	ambient.z = (*it).asFloat();

	col = root["diffuse"];
	it = col.begin();
	diffuse.x = (*it++).asFloat();
	diffuse.y = (*it++).asFloat();
	diffuse.z = (*it).asFloat();

}
