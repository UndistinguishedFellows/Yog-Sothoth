#include "Octree.h"

Octree::Octree()
{
}

Octree::~Octree()
{
}

void Octree::Create(const AABB aabb)
{
	root = new OctreeNode(nullptr, aabb);
}

void Octree::Insert(GameObject* object)
{
	if (root && object)
	{
		if (object->aabb.Intersects(root->aabb))
		{
			root->Insert(object);
		}
	}
}