﻿#include "ImportFBX.h"
#include "MeshImporter.h"
#include "../../Application.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include "../../Engine/GameObjects/Components/C_Transform.h"
#include "../../Engine/GameObjects/Components/C_Mesh.h"

namespace fs = std::experimental::filesystem;

bool ImportFBX::Import(fs::path path)
{
	oldPath = path;
	importPath.append(path.filename().string());

	//-----------------------------------------------------------------------------------------------------
	clock_t start, end;
	start = clock();

	infile.open(path.generic_string(), std::ios::binary);
	out.open(importPath, std::ios::binary);

	// file size
	infile.seekg(0, std::ios::end);
	length = infile.tellg();
	infile.seekg(0, infile.beg);

	// allocate memory for buffer
	buffer = new char[length];

	// copy file    
	infile.read(buffer, length);
	out.write(buffer, length);

	// clean up
	
	infile.close();
	out.close();

	end = clock();

	std::cout << "CLOCKS_PER_SEC " << CLOCKS_PER_SEC << "\n";
	std::cout << "CPU-TIME START " << start << "\n";
	std::cout << "CPU-TIME END " << end << "\n";
	std::cout << "CPU-TIME END - START " << end - start << "\n";
	std::cout << "TIME(SEC) " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "\n";
	//-----------------------------------------------------------------------------------------------------
	
	//Copies the file to assets folder
//	infile.open(path.generic_string(), std::ifstream::binary);
	//size
//	infile.seekg(0, std::ios::end);
//	length = infile.tellg();
//	infile.seekg(0, infile.beg);
//	buffer = new char[length+1];
//	std::string line;
//	int head = 0;
//	while (std::getline(infile, line))
//	{
//		memcpy(&buffer[head], line.c_str(), line.size());
//		head += line.size();
//	}
//	buffer[head] = '\0';
//	infile.close();
//
//	out.open(importPath, std::ofstream::binary);
//	out << buffer;
//	out.close();

	//Load the FBX
	if (Load())
	{
		Save();
	}
	//Save the resources and prefab
	


	RELEASE_ARRAY(buffer);
	return true;
}

bool ImportFBX::Load()
{
	bool ret = false;
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
			LoadMeshes(scene);
			LoadScene(scene, scene->mRootNode);
			ret = true;
		}
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error loading FBX, path: %s.", importPath.c_str());
	}

	aiReleaseImport(scene);
	return ret;
}

void ImportFBX::Save()
{
	for (auto mesh : meshes)
	{
		mesh->SaveMeshFile();
	}
	std::string name = ("data/assets/");
	name.append(oldPath.stem().string());
	root->children[0]->name.assign(name);
	name.append(".prefab");	
	root->children[0]->Save(name);
}

void ImportFBX::LoadMeshes(const aiScene* scene)
{
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		R_Mesh* mesh = new R_Mesh();
		mesh->Load(scene->mMeshes[i]);
		meshes.push_back(mesh);
	}	
}

void ImportFBX::LoadScene(const aiScene* scene, const aiNode* node)
{
	root = new GameObject();
	root->Transform = new C_Transform(root);
	root->Transform->localTransform.SetIdentity();
	root->name = "Root";

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
			GameObject* parent = treeMap[top->mParent];
			parent->AddChild(gameObject);
		}
		else
		{
			root->AddChild(gameObject);
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

		gameObject->Transform = transform;

		if (gameObject->parent != nullptr)
		{
			C_Transform* parentTransform = gameObject->parent->Transform;
			transform->globalTransform = parentTransform->globalTransform * transform->localTransform;
		}

		//Each mesh in a separate gameObject
		if (top->mNumMeshes > 1)
		{
			for (uint i = 0; i < node->mNumMeshes; i++)
			{
				GameObject* goMesh = new GameObject();
				std::string meshName = gameObject->name;
				meshName.append("_Mesh_%d", i);
				//const aiMesh* ai_mesh = scene->mMeshes[top->mMeshes[i]];

				C_Mesh* mesh = new C_Mesh(goMesh);

				mesh->rMesh = meshes[top->mMeshes[i]];
				goMesh->Mesh = mesh;
				gameObject->AddChild(goMesh);
			}
		}
		else if (top->mNumMeshes == 1)
		{
			const aiMesh* ai_mesh = scene->mMeshes[top->mMeshes[0]];

			C_Mesh* mesh = new C_Mesh(gameObject);

			mesh->rMesh = meshes[top->mMeshes[0]];

			gameObject->Mesh = mesh;
		}

		nodes.pop();
		for (int it = 0; it < top->mNumChildren; ++it)
		{
			nodes.push(top->mChildren[it]);
		}
	}


}