#include "ImportFBX.h"
#include "../MeshImporter.h"
#include "../../Application.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include "../../Engine/GameObjects/Components/C_Transform.h"

namespace fs = std::experimental::filesystem;

bool ImportFBX::Import(fs::path path)
{
	oldPath = path;
	importPath.append(path.filename().string());

	//Copies the file to assets folder
	infile.open(path.generic_string(), std::ifstream::binary);
	//size
	infile.seekg(0, infile.end);
	length = infile.tellg();
	infile.seekg(0, infile.beg);

	buffer = new char[length];
	infile.read(buffer, length);
	infile.close();

	out.open(importPath);
	out << buffer;
	out.close();


	Load();


	RELEASE_ARRAY(buffer);
	return true;
}

void ImportFBX::Load()
{
	yogConsole(CONSOLE_INFO, "Loading scene...");
	const aiScene* scene = nullptr;


	if (length > 0)
	{
		scene = aiImportFileFromMemory(buffer, length, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
	}
	if (scene != nullptr)
	{
		if (scene, scene->HasMeshes())
		{
			yogConsole(CONSOLE_INFO, "FBX path: %s.", importPath);
			LoadScene(scene, scene->mRootNode);
		}
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error loading FBX, path: %s.", importPath);
	}

	aiReleaseImport(scene);
}

void ImportFBX::LoadScene(const aiScene* scene, const aiNode* node)
{
	std::stack<const aiNode*> nodes;
	aiVector3D ai_translation;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;

	std::map<const aiNode*, GameObject*> treeMap;

	nodes.push(node);
	while (!nodes.empty())
	{
		const aiNode* top = nodes.top();

		top->mTransformation.Decompose(ai_scaling, ai_rotation, ai_translation);

		GameObject* gameObject = App->objManager->CreateGameObject();
		// insert pair into the tree
		std::pair<const aiNode*, GameObject*> pair;	pair.first = top; pair.second = gameObject;	treeMap.insert(pair);

		if (treeMap.find(top->mParent) != treeMap.end())
		{
			gameObject->parent = treeMap[top->mParent];
		}
		else
		{
			gameObject->parent = nullptr;
		}

		gameObject->name = top->mName.C_Str();
		float3 position(ai_translation.x, ai_translation.y, ai_translation.z);
		float3 scale(ai_scaling.x, ai_scaling.y, ai_scaling.z);
		Quat rotation(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);

		float4x4 matrix(rotation, position);
		matrix.Scale(scale);
		
		C_Transform* transform = new C_Transform(gameObject);
		transform->scale = scale;
		transform->rotation = rotation;
		transform->position = position;
		transform->localTransform = matrix;

		C_Transform* parentTransform = gameObject->parent->Transform;
		transform->globalTransform = parentTransform->globalTransform * transform->localTransform;

		




		nodes.pop();
		for (int it = 0; it < top->mNumMeshes; ++it)
		{
			nodes.push(top->mChildren[it]);
		}
	}


}
