#include "ImportFBX.h"
#include "MeshImporter.h"
#include "../../Application.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include "../../Engine/GameObjects/Components/C_Transform.h"
#include "../../Engine/GameObjects/Components/C_Mesh.h"
#include "Static/JsonSerializer.h"

namespace fs = std::experimental::filesystem;

bool ImportFBX::Import(fs::path path)
{
	oldPath = path;
	importPath.append(path.filename().string());

	//-----------------------------------------------------------------------------------------------------
	infile.open(path.generic_string(), std::ios::binary);
	

	// file size
	infile.seekg(0, std::ios::end);
	length = infile.tellg();
	infile.seekg(0, infile.beg);

	// allocate memory for buffer
	buffer = new char[length];

	// copy file    
	infile.read(buffer, length);
	out.open(importPath, std::ios::binary);
	out.write(buffer, length);

	// clean up
	
	infile.close();
	out.close();
	//-----------------------------------------------------------------------------------------------------
	std::string metName("data/assets/");
	metName.append(importPath.stem().string());
	metName.append(".meta");
	std::ifstream meta(metName);
	std::string line;

	//Delete old resources
	while (std::getline(meta, line))
	{
		std::string input(line);
		Json::Value deserializeRoot;
		Json::Reader reader;

		if(reader.parse(input, deserializeRoot))
		{
			std::string uuid = deserializeRoot.get("resource", 0).asString();
			std::string name("data/library/");
			name.append(uuid);
			fs::path p = name;
			std::string nameMeta = p.stem().string();
			nameMeta.append(".meta");
			std::remove(name.c_str());
			std::remove(nameMeta.c_str());
		}

	}

	//Load the FBX
	if (Load())
	{
		Save();
		//TEMP
		std::string name = ("data/assets/");
		name.append(oldPath.stem().string());
		name.append(".prefab");
		GameObject* go = new GameObject(App->objManager->root);
		go->name = "testingGO";
		go->CreateComponent(C_TRANSFORM);
		JsonSerializer::DeserializeFormPath(go, name);
		//TEMP
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
			LoadMaterials(scene);
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
		mesh->AddMeta(importPath.filename().string());
	}
	for (auto material : materials)
	{
		//material->SaveMeshFile();
		material->AddMeta(importPath.filename().string());
	}
	std::string name = ("data/assets/");
	name.append(oldPath.stem().string());
	root->children[0]->name.assign(oldPath.stem().string());
	name.append(".prefab");	
	root->children[0]->Save(name);
	
	name = ("data/assets/");
	name.append(oldPath.stem().string());
	name.append(".meta");
	std::ofstream meta(name);
	for (int i = 0; i < meshes.size(); i++)
	{
		meta << "{ \"resource\" : \"" << meshes[i]->uuid << ".mesh\"}" << std::endl;
	}
	for (int i = 0; i < materials.size(); i++)
	{
		meta << "{ \"resource\" : \"" << materials[i]->uuid << ".dds\"}" << std::endl;
	}
	meta.close();
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

void ImportFBX::LoadMaterials(const aiScene* scene)
{
	if (scene->HasMaterials())
	{
		aiMaterial** mat = scene->mMaterials;
		int i = 0;
		while (i < scene->mNumMaterials)
		{
			aiString ai_path;
			mat[i]->GetTexture(aiTextureType_DIFFUSE, 0, &ai_path);
			fs::path p_material = ai_path.C_Str();
			std::string f_name("data/assets/");
			f_name.append(p_material.filename().string());
			yogConsole(CONSOLE_INFO, "Texture name: %s", f_name.c_str());
			std::string olp(oldPath.parent_path().string());
			olp.append("\\");
			olp.append(p_material.filename().string());
			yogConsole(CONSOLE_INFO, "Texture path: %s", olp.c_str());

			std::ifstream input(olp, std::ios::binary);
			// file size
			input.seekg(0, std::ios::end);
			int length = input.tellg();
			input.seekg(0, input.beg);

			// allocate memory for buffer
			char* buffer = new char[length];

			// copy file    
			input.read(buffer, length);
			input.close();
			std::ofstream out(f_name, std::ios::binary);
			out.write(buffer, length);
			out.close();
			//if (strcmp(f_name.c_str(), "data/assets/") != 0)
			{
				R_Material* material = new R_Material();

				material->SaveMaterialFile(f_name.c_str());
				materials.push_back(material);
			}


			++i;
		}
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
		
		C_Transform* transform = gameObject->Transform;
		transform->scale = scale;
		transform->rotation = rotation;
		transform->position = position;
		transform->localTransform = matrix;

		//gameObject->Transform = transform;

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
			C_Mesh* mesh = new C_Mesh(gameObject);
			mesh->rMesh = meshes[top->mMeshes[0]];
			gameObject->Mesh = mesh;
			int i = scene->mMeshes[top->mMeshes[0]]->mMaterialIndex;

			if(scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				C_Material* material = new C_Material(gameObject);
				material->rMaterial = materials[i];
				gameObject->Material = material;
				gameObject->Mesh->associatedMaterial = material;
			}
						
		}


		nodes.pop();
		for (int it = 0; it < top->mNumChildren; ++it)
		{
			nodes.push(top->mChildren[it]);
		}
	}

	


}
