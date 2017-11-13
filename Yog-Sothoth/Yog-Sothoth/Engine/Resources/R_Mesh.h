#pragma once
#include "../../Globals.h"
#include "../../Tools/Interfaces/IJsonSerializable.h"

struct aiMesh;

struct Indices
{
	uint idIndices = 0;
	uint numIndices = 0;
	uint* indices = nullptr;
};
struct Vertices
{
	uint idVertices = 0;
	uint numVertices = 0;
	float* vertices = nullptr;
};
struct Normals
{
	uint idNormals = 0;
	uint numNormals = 0;
	float* normals = nullptr;
};
struct UV
{
	uint idUV = 0;
	uint numUV = 0;
	float* uv = nullptr;
};

class R_Mesh : public IJsonSerializable
{
public:
	UID uid;
	unsigned int type = 1;

	uint VAO;
	Indices indices;
	Vertices vertices;
	Normals normals;
	UV uv;

	R_Mesh();
	~R_Mesh();

	void SaveMeshFile();
	void LoadMeshFile(const char* fileName);
	void Load(const aiMesh* aiMesh);
	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};
