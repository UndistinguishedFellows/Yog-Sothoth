#pragma once
#include "../../Globals.h"
#include "../../../Tools/Containers/Color.h"
#include "../../../MathGeoLib/MathGeoLib.h"
#include "BaseClass/Component.h"
#include "../../../Tools/Containers/Shader.h"
#include "C_Camera.h"

class GameObject;
struct aiMesh;

struct Indices
{
	uint idIndices = 0;
	uint numIndices = 0;
	uint* indices = nullptr;
};
struct Vertices
{
	uint idVertices = 0;
	uint numVertices = 0;
	float* vertices = nullptr;
};
struct Normals
{
	uint idNormals = 0;
	uint numNormals = 0;
	float* normals = nullptr;
};
struct UV
{
	uint idUV = 0;
	uint numUV = 0;
	float* uv = nullptr;
};
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
	uint VAO;
	Indices indices;
	Vertices vertices;
	Normals normals;
	UV uv;
	BoundingBox boundingBox;
	Color color = Color(1.0f,1.0f,1.0f,1.0f);
	bool drawNormals;
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

	void UpdateBoundingBoxes();
	void Draw_OBB();
	void Draw_AABB();

	void Serialize(Json::Value& root);
	void Deserialize(Json::Value& root);
	

};

