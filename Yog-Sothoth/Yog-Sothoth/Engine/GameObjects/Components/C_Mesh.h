#pragma once
#include "../../Globals.h"
#include "../../../Tools/Containers/Color.h"
#include "../../../MathGeoLib/MathGeoLib.h"
#include "BaseClass/Component.h"
#include "../../../Tools/Containers/Shader.h"
#include "C_Camera.h"
#include "C_Material.h"
#include "../../Resources/R_Mesh.h"

class GameObject;
struct aiMesh;

struct BoundingBox
{
	OBB obb;
	AABB aabb;

	Color obb_color;
	Color aabb_color;

};

class C_Mesh : public Component
{
public:
	//Shader shader;
	R_Mesh* rMesh = nullptr;
	BoundingBox boundingBox;
	Color color = Color(1.0f,1.0f,1.0f,1.0f);
	bool drawNormals = false;
	bool wireframe = false;
	C_Material* associatedMaterial = nullptr;

private:
	OBB obb;
	AABB aabb;

	Color obb_color;
	Color aabb_color;

public:
	C_Mesh(GameObject* parent);
	~C_Mesh();

	void Update();
	void Load(const aiMesh* mesh);
	void Draw(Shader shader, C_Camera* camera) const;
	void DrawNormals(Shader shader, C_Camera* camera) const;
	void DrawSelected(Shader shader, C_Camera* camera) const;

	void UpdateBoundingBoxes();
	void Draw_OBB();
	void Draw_AABB();
	AABB GetAABB();

	void Serialize(Json::Value& root);
	void Deserialize(Json::Value& root);
	

};