#pragma once
#include <vector>
#include "Components/BaseClass/Component.h"
#include <rpc.h>

enum GameObjectType
{
	GO_STANDARD,
	GO_LIGHT	
};

class Shader;
class C_Camera;

class GameObject
{
public:
	GameObjectType type = GO_STANDARD;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	std::vector<Component*> components;

	// Todo: for now i dont know if this is really necessary, maybe I just dont need to know relationships
	std::vector<GameObject**> relationship; 

	std::string name = "NO_NAME";
	UUID uuid;

	bool active = true;
	bool selected = false;

public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	/**
	 * \brief Add a child into children list of this object
	 * \param child Child to be added
	 */
	void AddChild(GameObject* child);
	/**
	 * \brief Will delete the child too
	 * \param child Child to be Removed
	 */
	void RemoveChild(GameObject* child);
	/**
	 * \brief Will NOT delete the child
	 * \param child Child to be erased
	 */
	void EraseChild(GameObject* child);  
	/**
	 * \brief Will delete all the children and their children recursively
	 */
	void RemoveChildren(); //Recursive
	/**
	 * \brief Search for a child in hieracy from object as reference
	 * \param go GameObject to find
	 * \return Bool true if found
	 */
	bool FindChild(GameObject* go);
	/**
	 * \brief 
	 * \param goName Find a GO with a specific name, only the first one with the same name will be returned, so try to dont repeat names.
	 * \return Returns the first coincident GO
	 */
	GameObject* FindChild(std::string goName);
	/**
	 * \brief Adds a new relationship for this game object
	 * \param reference the new object that references this one
	 */
	void AddRelationship(GameObject** reference);

	/**
	 * \brief Erase a relationship from relationship vector
	 * \param reference the relationship to remove
	 */
	void EraseRelationship(GameObject** reference);

	Component* CreateComponent(ComponentType type);
	Component* FindComponent(ComponentType type);
	void Draw(Shader shader, C_Camera* camera);
	void DrawLight(Shader shader, C_Camera* camera);
};
