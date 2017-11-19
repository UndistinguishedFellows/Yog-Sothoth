#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "../../Engine/GameObjects/GameObject.h"

#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include "../Engine/Resources/R_Mesh.h"
#include "../Engine/Resources/R_Material.h"
namespace fs = std::experimental::filesystem;

class ImportFBX
{
public:

	bool Import(fs::path path);

	bool Load();
	void Save();
	void LoadMeshes(const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	void LoadScene(const aiScene * scene, const aiNode * node);

	fs::path importPath;
private:
	fs::path oldPath;
	std::ifstream infile;
	std::ofstream out;
	std::ifstream::pos_type length;
	char * buffer = nullptr;
	GameObject* root;
	std::vector<R_Mesh*> meshes;
	std::vector<R_Material*> materials;
};
