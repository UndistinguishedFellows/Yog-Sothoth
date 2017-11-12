#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "../../Engine/GameObjects/GameObject.h"

#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include "../Engine/Resources/R_Mesh.h"
namespace fs = std::experimental::filesystem;

class ImportFBX
{
public:
	bool Import(fs::path path);

	void Load();
	void Save();
	void LoadMeshes(const aiScene* scene);
	void LoadScene(const aiScene * scene, const aiNode * node);

	fs::path importPath;
private:
	fs::path oldPath;
	std::ifstream infile;
	std::ofstream out;
	int length;
	char * buffer;
	GameObject* root;
	std::vector<R_Mesh*> meshes;
};
