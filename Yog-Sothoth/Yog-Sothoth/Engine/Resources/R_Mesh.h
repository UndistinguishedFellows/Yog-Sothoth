#pragma once
#include "../GameObjects/Components/C_Mesh.h"

class R_Mesh : public IJsonSerializable
{
public:
	UID uid;

	unsigned int type = 1;

	Indices indices;
	Vertices vertices;
	Normals normals;
	UV uv;

	R_Mesh();
	~R_Mesh();

	void SaveMeshFile();
	void LoadMeshFile(const char* fileName);
	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};
