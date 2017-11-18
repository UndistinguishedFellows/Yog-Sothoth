#pragma once
#include "../MathGeoLib/MathGeoLib.h"
#include "../Engine/GameObjects/GameObject.h"

class OctreeNode
{
public:
	AABB aabb;
	std::vector<GameObject*> objects;
	OctreeNode* parent = nullptr;
	OctreeNode* childs[8];

	OctreeNode(OctreeNode* _parent, const AABB& _aabb);
	~OctreeNode();

	void Insert(GameObject* object);
	void Split();
	void Reorder();
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const;
};