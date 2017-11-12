#pragma once
#include "../GameObjects/Components/C_Material.h"

class R_Material : public IJsonSerializable
{
public:
	UID uid;

	unsigned int type = 2;

	R_Material();
	~R_Material();

	void SaveMaterialFile();
	void LoadMaterialFile(const char* fileName);
	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};