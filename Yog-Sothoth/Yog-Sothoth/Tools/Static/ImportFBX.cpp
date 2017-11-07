#include "ImportFBX.h"
#include "../MeshImporter.h"
#include "../../Application.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"

bool ImportFBX::Import(const char* path)
{
	MeshImporter impMesh;
	//MaterialImporter material;

	bool ret = false;
	yogConsole(CONSOLE_INFO, "Loading scene...");
	if (path == NULL)
	{
		yogConsole(CONSOLE_ERROR, "No path");
		return nullptr; //If path is NULL dont do nothing
	}

	char* buffer;
	//TODO Dont use PhysFS
	uint fileSize = App->fs->Load(path, &buffer);
	const aiScene* scene = nullptr;

	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
		ret = true;
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
		ret = false;
	}
	impMesh.Import(scene);
	//MaterialImporter

	aiReleaseImport(scene);
	return ret;
	//impMesh.Import(path, UUID);
}
