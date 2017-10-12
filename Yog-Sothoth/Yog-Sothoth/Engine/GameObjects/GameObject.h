#pragma once
#include <vector>
#include "Components/BaseClass/Component.h"
#include <rpc.h>
#include <list>
#include "../../MathGeoLib/Geometry/AABB.h"

enum GameObjectType
{
	GO_STANDARD,
	GO_LIGHT	
};

class Shader;
class C_Camera;
class C_Mesh;
class C_Transform;
class C_Light;
class C_Material;
class M_GameObjectManager;

class GameObject
{
	friend class M_ObjectManager;
public:
	GameObjectType type = GO_STANDARD;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	std::vector<Component*> components;

	// Todo: for now i dont know if this is really necessary, maybe I just dont need to know relationships
	std::vector<GameObject**> relationship; 

	std::string name = "NO_NAME";
	UUID uuid;

	AABB aabb;

	bool active = true;
	bool selected = false;
private:
	//For now there will be only one attribute of each
	C_Mesh*			mesh		= nullptr;
	C_Transform*	transform	= nullptr;
	C_Camera*		camera		= nullptr;
	C_Material*		material	= nullptr;
	C_Light*		light		= nullptr;

protected:	
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
	static void MoveChild(GameObject* child, GameObject* origin, GameObject* destiny);

	std::vector<GameObject*> GetChildren();

	void LookAt(float3 pos);
	void SetPos(float3 pos);
	AABB UpdateBoundingBoxes();

	Component* CreateComponent(ComponentType type);
	Component* FindComponent(ComponentType type);
	void Draw(Shader shader, C_Camera* camera);
	void DrawNormals(Shader shader, C_Camera* camera);
	void DrawLight(Shader shader, C_Camera* camera);
	void Draw_AABB();
	AABB GetAABB();

public:
	C_Mesh* mesh1() const { return mesh; }
	void set_mesh(C_Mesh* const mesh) { this->mesh = mesh; }
	__declspec(property(get = mesh1, put = set_mesh)) C_Mesh* Mesh;

	C_Transform* transform1() const { return transform; }
	void set_transform(C_Transform* const transform) { this->transform = transform; }
	__declspec(property(get = transform1, put = set_transform)) C_Transform* Transform;

	C_Camera* camera1() const { return camera; }
	void set_camera(C_Camera* const camera) { this->camera = camera; }
	__declspec(property(get = camera1, put = set_camera)) C_Camera* Camera;

	C_Material* material1() const { return material; }
	void set_material(C_Material* const material) { this->material = material; }
	__declspec(property(get = material1, put = set_material)) C_Material* Material;

	C_Light* light1() const { return light; }
	void set_light(C_Light* const light) { this->light = light; }
	__declspec(property(get = light1, put = set_light)) C_Light* Light;


};
