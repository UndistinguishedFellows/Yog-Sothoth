#pragma once
#include <vector>
#include "Components/BaseClass/Component.h"
#include <rpc.h>
#include <list>
#include "../../MathGeoLib/Geometry/AABB.h"
#include "../../Globals.h"

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

class GameObject : public IJsonSerializable
{
	friend class M_ObjectManager;
	friend class ImportFBX;
public:
	GameObjectType type = GO_STANDARD;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	std::vector<Component*> components;

	std::string name = "NO_NAME";
	UUID32 uuid;
	UUID32 parent_uuid;

	AABB aabb;

	bool active = true;
	bool selected = false;
	bool drawAABB = false;
	bool isStatic = false;
	bool serializable = true;

	std::pair<std::string, Shader*> shader;

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

	void DoPreupdate();
	void DoUpdate();
	void DoPostUpdate();

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
	void RefreshTransform();

	Component* CreateComponent(ComponentType type);
	void DeleteComponent(ComponentType type);
	Component* FindComponent(ComponentType type);
	void SendToDraw();
	void RemoveFromDraw();
	void Draw(Shader shader, C_Camera* camera);
	void DrawNormals(Shader shader, C_Camera* camera);
	void DrawLight(Shader shader, C_Camera* camera);
	void DrawAABB(Shader shader, C_Camera* camera);
	AABB GetAABB();

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;

	void Save(std::string fileName);
	void Load(std::string fileName);


public:
	C_Mesh* mesh1() const { return mesh; }
	void set_mesh(C_Mesh* const mesh)
	{
		C_Mesh* comp = (C_Mesh*)FindComponent(C_MESH);
		for (std::vector<Component*>::iterator it = components.begin();
			 it != components.end(); ++it)
			if (comp == (C_Mesh*)(*it))
				components.erase(it);
		this->mesh = mesh;
		components.push_back((Component*)mesh);
	}
	__declspec(property(get = mesh1, put = set_mesh)) C_Mesh* Mesh;

	C_Transform* transform1() const { return transform; }
	void set_transform(C_Transform* const transform)
	{
		C_Transform* comp = (C_Transform*)FindComponent(C_TRANSFORM);
		for (std::vector<Component*>::iterator it = components.begin();
			 it != components.end(); ++it)
			if (comp == (C_Transform*)(*it))
				components.erase(it);
		this->transform = transform;
		components.push_back((Component*)transform);
	}
	__declspec(property(get = transform1, put = set_transform)) C_Transform* Transform;

	C_Camera* camera1() const { return camera; }
	void set_camera(C_Camera* const camera)
	{
		C_Camera* comp = (C_Camera*)FindComponent(C_CAMERA);
		for (std::vector<Component*>::iterator it = components.begin();
			 it != components.end(); ++it)
			if (comp == (C_Camera*)(*it))
				components.erase(it);
		this->camera = camera;
		components.push_back((Component*)camera);
	}
	__declspec(property(get = camera1, put = set_camera)) C_Camera* Camera;

	C_Material* material1() const { return material; }
	void set_material(C_Material* const material)
	{
		C_Material* comp = (C_Material*)FindComponent(C_MATERIAL);
		for (std::vector<Component*>::iterator it = components.begin();
			 it != components.end(); ++it)
			if (comp == (C_Material*)(*it))
				components.erase(it);
		this->material = material;
		components.push_back((Component*)material);
	}
	__declspec(property(get = material1, put = set_material)) C_Material* Material;

	C_Light* light1() const { return light; }
	void set_light(C_Light* const light)
	{
		C_Light* comp = (C_Light*)FindComponent(C_LIGHT);
		for (std::vector<Component*>::iterator it = components.begin();
			 it != components.end(); ++it)
			if (comp == (C_Light*)(*it))
				components.erase(it);
		this->light = light;
		components.push_back((Component*)light);
	}
	__declspec(property(get = light1, put = set_light)) C_Light* Light;


};
