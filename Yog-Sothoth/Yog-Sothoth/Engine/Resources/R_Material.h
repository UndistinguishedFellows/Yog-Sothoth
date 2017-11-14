#pragma once
#include "../../Globals.h"
#include "../../Tools/Interfaces/IJsonSerializable.h"

class R_Material : public IJsonSerializable
{
public:
	UUID32 uuid;

	unsigned int type;

	R_Material();
	~R_Material();

	void SaveMaterialFile();
	void LoadMaterialFile(const char* fileName);
	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};
