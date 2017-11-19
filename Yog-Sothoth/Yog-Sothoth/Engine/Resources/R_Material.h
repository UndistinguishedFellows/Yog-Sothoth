#pragma once
#include "../../Globals.h"
#include "../../Tools/Interfaces/IJsonSerializable.h"
#include "Resource.h"

class R_Material : public Resource, public IJsonSerializable
{
public:
	UUID32 uuid;

	uint texture;

	R_Material();
	~R_Material();

	void SaveMaterialFile(const char* path);
	void LoadMaterialFile(const char* fileName);
	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void AddMeta(std::string prefab);
	void RemoveMeta(std::string prefab);

};
