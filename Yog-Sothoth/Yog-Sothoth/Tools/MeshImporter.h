#pragma once
#include "../Engine/GameObjects/Components/C_Mesh.h"
#include "../../Assimp/Assimp/include/scene.h"

class MeshImporter
{
public:
	const aiScene* scene = nullptr;

	bool Import(const aiScene* scene);
};
