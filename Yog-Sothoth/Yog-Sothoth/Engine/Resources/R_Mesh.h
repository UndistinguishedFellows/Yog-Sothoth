#pragma once
#include "../GameObjects/Components/C_Mesh.h"

class R_Mesh : public IJsonSerializable
{
public:
	UID uid;

	Indices indices;
	Vertices vertices;
	Normals normals;
	UV uv;



	R_Mesh();
	~R_Mesh();

	void MakeMeshFile();
	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;

};
