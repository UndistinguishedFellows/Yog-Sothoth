#pragma once
#include "../CoreModules/BaseClass/Module.h"
#include "GameObject.h"
#include "Components/C_Light.h"
#include <list>
#include "../../Tools/Primitives.h"

struct aiScene;
struct aiNode;

class M_ObjectManager :
	public Module
{
public:
	/**
	 * \brief The root Game Object that stores all GO
	 */
	GameObject* focus = nullptr;
	GameObject* root = nullptr;
	GameObject* dragAndDropVisualizer = nullptr;
	GameObject* testLight;

	//Main Camera
	GameObject* camera = nullptr;
	//Active camera
	GameObject* activeCamera = nullptr;

	std::vector<GameObject*> lights;
	std::vector<GameObject*> cameras;

	Primitives::Primitives* primitives;

private:
	
	/**
	 * \briefGame object pointers allocated here will be deleted in the next frame
	 */
	GameObject* deletionGameObject; 

public:
	M_ObjectManager(bool enabled = true);
	~M_ObjectManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	static GameObject* CreateGameObject(GameObject* parent = nullptr); //Create an object and if a parent is set will add it into the parents list.
	GameObject* CreateGameObject(std::string parentName, std::string childName) const;
	void ConsoleCreateGameObject(std::string parentName, std::string childName) const;
	void DeleteGameObject(GameObject* go);
	GameObject* GetFocusGO();
	void SetFocusGO(GameObject* go);
	GameObject* FindGameObject(std::string name);
	void UpdateBoundingBoxes(GameObject* go = nullptr);
	
	/**
	* \brief  TODO:Temporal method to load a FBX, this must be removed when resource manager will be implementated
	*/
	GameObject* LoadFBX(const char* path);
	GameObject* LoadFBXFromDragAndDrop(const char* path, const char* oldPath = nullptr);
	/**
	 * \brief  TODO:Temporal method to load a FBX, this must be removed when resource manager will be implementated
	 */
	void LoadScene(const aiScene * scene, const aiNode * node, GameObject * parent, const char* oldPath = nullptr);

	void Draw(GameObject* drawFrom, Shader shader) const;
	void DrawNormals(GameObject* drawFrom, Shader shader) const;
	void DrawAABB(GameObject* drawFrom, Shader shader) const;

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig() override;
	void SaveConfig() override;

	
};

