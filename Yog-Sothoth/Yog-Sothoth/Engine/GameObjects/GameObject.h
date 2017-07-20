#pragma once
#include <vector>

class GameObject
{
public:
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	bool active = true;

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
	 * \brief 
	 * \param go GameObject to find
	 * \return Int means the deepness from the searching reference
	 */
	std::pair<int, GameObject*> FindChild(GameObject* go);
};
