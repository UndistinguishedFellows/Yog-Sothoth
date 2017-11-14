#include "R_Material.h"
#include <fstream>

R_Material::R_Material()
{
}

R_Material::~R_Material()
{
}

void R_Material::SaveMaterialFile()
{
	LCG lcg;
	uuid = lcg.Int();

	std::string filename = std::to_string(uuid);
	filename.append(".dds"); 
}

void R_Material::LoadMaterialFile(const char* filename)
{
}

void R_Material::Serialize(Json::Value& root)
{
}

void R_Material::Deserialize(Json::Value& root)
{
}