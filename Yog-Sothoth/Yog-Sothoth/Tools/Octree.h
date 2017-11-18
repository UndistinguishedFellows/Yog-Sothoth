#pragma once
#include "OctreeNode.h"

class Octree
{
public:
	OctreeNode* root;

	Octree();
	~Octree();

	void Create(const AABB aabb);
	//void Clear();
	void Insert(GameObject* object);
	void Remove();
	void Intersect();
};