#include "M_ObjectManager.h"
#include "../../MathGeoLib/MathGeoLib.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include "Components/C_Mesh.h"
#include "Components/C_Transform.h"
#include "../../Application.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include <experimental/filesystem>
#include <fstream>
#include <iterator>


namespace fs = std::experimental::filesystem;

M_ObjectManager::M_ObjectManager(bool enabled): Module(enabled), deletionGameObject(nullptr)
{
	name = "objManager";
}


M_ObjectManager::~M_ObjectManager()
{
	RELEASE(primitives);
}

bool M_ObjectManager::Init()
{

	return true;
}

bool M_ObjectManager::Start()
{

	deletionGameObject = new GameObject();
	deletionGameObject->name = "deletionGameObject";

	root = new GameObject();
	root->name = "/";
	root->CreateComponent(C_TRANSFORM);

	dragAndDropVisualizer = new GameObject();
	dragAndDropVisualizer->name = "drag&drop";
	root->AddChild(dragAndDropVisualizer);
	dragAndDropVisualizer->CreateComponent(C_TRANSFORM);	

	camera = new GameObject();
	camera->name = "camera";
	root->AddChild(camera);
	camera->CreateComponent(C_CAMERA);
	camera->CreateComponent(C_TRANSFORM);	
	camera->SetPos(float3(0, 5.f, 10.f));
	camera->Transform->SetRotation(0, 180, 0);	

	testLight = new GameObject();
	testLight->name = "testLight";
	root->AddChild(testLight);
	testLight->CreateComponent(C_LIGHT);
	testLight->CreateComponent(C_TRANSFORM);
	testLight->type = GO_LIGHT;
	testLight->Transform->SetPosition(float3(0.f, 20.0f, 5.0f));
	
	activeCamera = camera;

	primitives = new Primitives::Primitives();

	glGenVertexArrays(1, &primitives->pCube.VAO);
	glGenBuffers(1, (GLuint*) &(primitives->pCube.vertices.idVertices));
	glGenBuffers(1, (GLuint*) &(primitives->pCube.indices.idIndices));
	glBindVertexArray(primitives->pCube.VAO);
	//vertices	
	glBindBuffer(GL_ARRAY_BUFFER, primitives->pCube.vertices.idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * primitives->pCube.vertices.numVertices * 3, primitives->pCube.vertices.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//indices	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitives->pCube.indices.idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * primitives->pCube.indices.numIndices, primitives->pCube.indices.indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);










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
	uint fileSize = App->fs->Load(path, &buffer);
	const aiScene* scene = nullptr;

	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
		return nullptr;
	}
	if (scene != nullptr)
	{
		if (scene, scene->HasMeshes())
		{
			yogConsole(CONSOLE_INFO, "FBX path: %s.", path);
			LoadScene(scene, scene->mRootNode, root);
		}

	}

	aiReleaseImport(scene);

	return root;
}

GameObject* M_ObjectManager::LoadFBXFromDragAndDrop(const char* path, const char* oldPath)
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

	//std::ifstream input(path, std::ios::in | std::ios::binary | std::ios::ate);
	size = App->fs->Load(path, &memblock);

	if (size > 0)
	{
//		size = input.tellg();
//		memblock = new char[size];
//		input.seekg(0, std::ios::beg);
//		input.read(memblock, size);
//		input.close();

		if (size > 0)
		{
			scene = aiImportFileFromMemory(memblock, size, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
		}
		else
		{
			yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
			return nullptr;
		}
	}
	RELEASE_ARRAY(memblock);
	if (scene != nullptr)
	{
		if (scene, scene->HasMeshes())
		{
			yogConsole(CONSOLE_INFO, "FBX path: %s.", path);
			LoadScene(scene, scene->mRootNode, dragAndDropVisualizer, oldPath);
		}
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error loading FBX, path: %s.", path);
	}

	aiReleaseImport(scene);

	return root;

}

void M_ObjectManager::LoadScene(const aiScene * scene, const aiNode * node, GameObject * parent, const char* _oldPath)
{
	aiVector3D ai_translation;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;
	std::string gameObjectName;

	node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_translation);
	if (strcmp(node->mName.C_Str(), "RootNode") == 0)
	{
		for (uint i = 0; i < node->mNumChildren; ++i)
			LoadScene(scene, node->mChildren[i], parent, _oldPath);

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

				std::string fullPath = "data/assets/";
				if (strcmp(ai_path.C_Str(), "") != 0)
				{
					if (_oldPath != nullptr)
					{
						std::string finalPath("data/assets/");
						std::experimental::filesystem::path oldPath(_oldPath);
						oldPath.append(ai_path.C_Str());
						finalPath.append(ai_path.C_Str());
						std::ifstream infile(oldPath, std::ifstream::binary);
						//size
						infile.seekg(0, infile.end);
						int length = infile.tellg();
						infile.seekg(0, infile.beg);

						char * buffer = new char[length];
						infile.read(buffer, length);
						infile.close();

						if (oldPath.extension().generic_string() == ".png")
						{
							yogConsole(CONSOLE_INFO, "Loading png");
							App->fs->save(finalPath.c_str(), buffer, length);

							fullPath.append(ai_path.data);
							material->LoadTexture(fullPath.c_str());
						}
						RELEASE_ARRAY(buffer);
					}
					

				}
				mesh->associatedMaterial = material;
			}


		}

		for (uint i = 0; i < node->mNumChildren; ++i)
			LoadScene(scene, node->mChildren[i], gameObject);
	}

}

void M_ObjectManager::Draw(GameObject* drawFrom, Shader shader) const
{
	drawFrom->Draw(shader, activeCamera->Camera);
}

void M_ObjectManager::DrawNormals(GameObject* drawFrom, Shader shader) const
{
	drawFrom->DrawNormals(shader, activeCamera->Camera);
}

void M_ObjectManager::DrawAABB(GameObject* drawFrom, Shader shader) const
{
	std::stack<GameObject*> stack;
	stack.push(drawFrom);
	while (!stack.empty())
	{
		GameObject* top = stack.top();

		if (top->drawAABB)
			top->DrawAABB(shader, activeCamera->Camera);

		stack.pop();
		for (int it = 0; it != top->children.size(); ++it)
		{
			stack.push(top->children[it]);
		}
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
