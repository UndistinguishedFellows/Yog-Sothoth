#include "M_ObjectManager.h"
#include "../../MathGeoLib/MathGeoLib.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include "Components/C_Mesh.h"
#include "Components/C_Transform.h"
#include "../../Application.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>


M_ObjectManager::M_ObjectManager(bool enabled): Module(enabled), deletionGameObject(nullptr)
{
	name = "objManager";
	deletionGameObject = new GameObject();
	deletionGameObject->name = "deletionGameObject";
	root = new GameObject();
	root->name = "/";
	root->CreateComponent(C_TRANSFORM);
	dragAndDropVisualizer = new GameObject();
	dragAndDropVisualizer->name = "drag&drop";
	dragAndDropVisualizer->CreateComponent(C_TRANSFORM);
	root->AddChild(dragAndDropVisualizer);

	camera = new GameObject();
	camera->name = "camera";
	camera->CreateComponent(C_CAMERA);
	camera->CreateComponent(C_TRANSFORM);

	root->AddChild(camera);
	
}


M_ObjectManager::~M_ObjectManager()
{
}

bool M_ObjectManager::Init()
{

	return true;
}

bool M_ObjectManager::Start()
{
	camera->SetPos(float3(0, 5.f, 10.f));
	C_Transform* transform = (C_Transform*)camera->FindComponent(C_TRANSFORM);
	transform->SetRotation(0, 180, 0);

	return true;
}
//Todo: Here game objects will be deleted when needed
update_status M_ObjectManager::PreUpdate(float dt)
{
	if (deletionGameObject->FindChild(focus))
	{
		focus = nullptr;
	}
	if (deletionGameObject->children.size() > 0)
	{
		deletionGameObject->RemoveChildren();
	}	
	return UPDATE_CONTINUE;
}

//Todo: Game objects logic
update_status M_ObjectManager::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status M_ObjectManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_ObjectManager::CleanUp()
{

	return true;
}

GameObject* M_ObjectManager::CreateGameObject(GameObject* parent)
{
	GameObject* gm = new GameObject();
	if (parent != nullptr)
	{
		gm->parent = parent;
		parent->AddChild(gm);
	}

	return gm;
}

GameObject* M_ObjectManager::CreateGameObject(std::string parentName, std::string childName) const
{
	GameObject* parent = root->FindChild(parentName);
	GameObject* child = CreateGameObject(parent);
	child->name = childName;
	return child;
}

void M_ObjectManager::ConsoleCreateGameObject(std::string parentName, std::string childName) const
{
	GameObject* parent = root->FindChild(parentName);
	GameObject* child = CreateGameObject(parent);
	child->name = childName;

}

void M_ObjectManager::DeleteGameObject(GameObject* go)
{
	deletionGameObject->AddChild(go);
}

GameObject* M_ObjectManager::GetFocusGO()
{
	return focus;
}

void M_ObjectManager::SetFocusGO(GameObject* go)
{
	focus = go;
}

GameObject* M_ObjectManager::FindGameObject(std::string name)
{
	return root->FindChild(name);
}

GameObject* M_ObjectManager::LoadFBX(const char * path)
{
	yogConsole(CONSOLE_INFO, "Loading scene...");
	if (path == NULL)
	{
		yogConsole(CONSOLE_ERROR, "No path");
		return nullptr; //If path is NULL dont do nothing
	}

	char* buffer;
	uint fileSize = App->fs->load(path, &buffer);
	const aiScene* scene = NULL;

	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
		return NULL;
	}

	if (scene, scene->HasMeshes())
	{
		yogConsole(CONSOLE_INFO, "FBX path: %s.", path);
		LoadScene(scene, scene->mRootNode, root);
	}

	aiReleaseImport(scene);

	return root;
}

GameObject* M_ObjectManager::LoadFBXFromDragAndDrop(const char* path)
{
	yogConsole(CONSOLE_INFO, "Loading scene...");
	if (path == NULL)
	{
		yogConsole(CONSOLE_ERROR, "No path");
		return nullptr; //If path is NULL dont do nothing
	}
	const aiScene* scene = nullptr;

	std::vector<GameObject*> childs = dragAndDropVisualizer->GetChildren();
	for (auto child : childs)
	{
		GameObject::MoveChild(child, dragAndDropVisualizer, deletionGameObject);
//		deletionGameObject->AddChild(child);
//		dragAndDropVisualizer->EraseChild(child);
	}

	std::streampos size;
	char * memblock = nullptr;

	std::ifstream input(path, std::ios::in | std::ios::binary | std::ios::ate);

	if (input.is_open())
	{
		size = input.tellg();
		memblock = new char[size];
		input.seekg(0, std::ios::beg);
		input.read(memblock, size);
		input.close();

		if (size > 0)
		{
			scene = aiImportFileFromMemory(memblock, size, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
		}
		else
		{
			yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
			return NULL;
		}
	}
	delete[] memblock;

	if (scene, scene->HasMeshes())
	{
		yogConsole(CONSOLE_INFO, "FBX path: %s.", path);
		LoadScene(scene, scene->mRootNode, dragAndDropVisualizer);
	}

	aiReleaseImport(scene);

	return root;

}

void M_ObjectManager::LoadScene(const aiScene * scene, const aiNode * node, GameObject * parent)
{
	aiVector3D ai_translation;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;
	std::string gameObjectName;

	node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_translation);
	if (strcmp(node->mName.C_Str(), "RootNode") == 0)
	{
		for (uint i = 0; i < node->mNumChildren; ++i)
			LoadScene(scene, node->mChildren[i], parent);

	}
	else
	{
		gameObjectName = node->mName.C_Str();
		float3 position(ai_translation.x, ai_translation.y, ai_translation.z);
		float3 scale(ai_scaling.x, ai_scaling.y, ai_scaling.z);
		Quat rotation(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);

		GameObject* gameObject = new GameObject();
		parent->AddChild(gameObject);
		gameObject->name = gameObjectName;

		float4x4 matrix(rotation, position);
		matrix.Scale(scale);

		C_Transform* transform = (C_Transform*)gameObject->CreateComponent(C_TRANSFORM);
		transform->scale = scale;
		transform->rotation = rotation;
		transform->position = position;
		transform->localTransform = matrix;

		C_Transform* parentTransform = (C_Transform*)gameObject->parent->FindComponent(C_TRANSFORM);

		transform->globalTransform = parentTransform->globalTransform * transform->localTransform;


		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			const aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];

			C_Mesh* mesh = (C_Mesh*)gameObject->CreateComponent(C_MESH);

			mesh->Load(ai_mesh);

			//PNG path && Filename
			if (scene->HasMaterials())
			{
				aiColor4D color;
				scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				C_Material* material = (C_Material*)gameObject->CreateComponent(C_MATERIAL);
				material->color.Set(color.r, color.g, color.b, color.a);
				aiString ai_path;
				std::string fileName;
				scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &ai_path);
				int j = 0;
				for (int i = ai_path.length - 1; i > 0; i--)
				{
					if (ai_path.data[i] != '/' && ai_path.data[i] != '\\')
					{
						j++;
					}
					else
						break;
				}
				fileName.assign(ai_path.C_Str() + ai_path.length - j,
								ai_path.C_Str() + ai_path.length);
				SDL_Log("Texture path: %s", ai_path.C_Str());
				SDL_Log("Texture name: %s", fileName.c_str());
				std::string fullPath = "data/assets/";
				fullPath.append(fileName);
				material->LoadTexture(fullPath.c_str());
				mesh->associatedMaterial = material;
			}


		}

		for (uint i = 0; i < node->mNumChildren; ++i)
			LoadScene(scene, node->mChildren[i], gameObject);
	}

}

void M_ObjectManager::Serialize(Json::Value& root)
{
}

void M_ObjectManager::Deserialize(Json::Value& root)
{
}

void M_ObjectManager::LoadConfig()
{
}

void M_ObjectManager::SaveConfig()
{
}
