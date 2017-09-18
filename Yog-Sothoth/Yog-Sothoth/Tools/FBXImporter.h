#pragma once

#include <vector>

struct VramVertex
{
	unsigned int VAO;

	unsigned int idVertices = 0;
	unsigned int numVertices = 0;
	float* vertices = nullptr;

	unsigned int idIndices = 0;
	unsigned int numIndices = 0;
	unsigned int* indices = nullptr;

	unsigned int idNormals = 0;
	unsigned int numNormals = 0;
	float* normals = nullptr;

	unsigned int idUV = 0;
	unsigned int numUV = 0;
	float* UV = nullptr;
};

class FBXImporter
{
public:

	std::vector<VramVertex*> meshes;

	FBXImporter();
	~FBXImporter();

	void LoadFBX(const char* path);
	void DrawMeshes(std::vector<VramVertex*> drawMeshes);

	
};

