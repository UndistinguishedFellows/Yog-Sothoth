#pragma once
#include "../CoreModules/BaseClass/Module.h"
#include "GameObject.h"

class M_ObjectManager :
	public Module
{
public:
	/**
	 * \brief The root Game Object that stores all GO
	 */
	GameObject* root;
	GameObject* camera;
private:
	
	/**
	 * \brief Game object pointers allocated here will be deleted in the next frame
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


	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig() override;
	void SaveConfig() override;

};

