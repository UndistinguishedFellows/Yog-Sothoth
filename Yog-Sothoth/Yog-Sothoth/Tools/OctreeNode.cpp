#include "OctreeNode.h"

#define MAX_OBJ_NODE 10

OctreeNode::OctreeNode(OctreeNode* _parent, const AABB& _aabb)
{
	parent = _parent;
	aabb = _aabb;

	for (int i = 0; i < 8; i++) {
		childs[i] = nullptr;
	}
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Insert(GameObject* object)
{
	if (!object) return;

	if (childs[0] == nullptr && objects.size() < MAX_OBJ_NODE)
	{
		objects.push_back(object);
	}
	else
	{
		if (childs[0] == nullptr)
		{
			Split();
		}

		objects.push_back(object);
		Reorder();
	}
}

void OctreeNode::Split()
{
	float3 octreeCenter = aabb.CenterPoint();
	float3 octreeSize = aabb.Size();

	float3 childCenterPos(0.0f, 0.0f, 0.0f);
	float3 childCenterSize(octreeSize.x / 4, octreeSize.y / 4, octreeSize.z / 4);
	float3 childSize(octreeSize.x / 2, octreeSize.y / 2, octreeSize.z / 2);
	AABB childAabb;

//	FRONT VIEW:					TOP VIEW:
//  ___________					 ___________
// |     |     |\   <-- UP		|     |     |  <-- BACK
// |_____|_____|||				|_____|_____|
// |     |     |||  <-- DOWN	|     |     |  <-- FRONT
// |_____|_____|/				|_____|_____|
//								 \____|____/
//							 LEFT--^	 ^--RIGHT

//### UP ###

//BACK-LEFT
	childCenterPos.Set(octreeCenter.x - childCenterSize.x, octreeCenter.y + childCenterSize.y, octreeCenter.z + childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[0] = new OctreeNode(this, childAabb);

//BACK-RIGHT
	childCenterPos.Set(octreeCenter.x + childCenterSize.x, octreeCenter.y + childCenterSize.y, octreeCenter.z + childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[1] = new OctreeNode(this, childAabb);

//FRONT-RIGHT
	childCenterPos.Set(octreeCenter.x + childCenterSize.x, octreeCenter.y + childCenterSize.y, octreeCenter.z - childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[2] = new OctreeNode(this, childAabb);

//FRONT-LEFT
	childCenterPos.Set(octreeCenter.x - childCenterSize.x, octreeCenter.y + childCenterSize.y, octreeCenter.z - childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[3] = new OctreeNode(this, childAabb);

//### DOWN ###

//BACK-LEFT
	childCenterPos.Set(octreeCenter.x - childCenterSize.x, octreeCenter.y - childCenterSize.y, octreeCenter.z + childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[4] = new OctreeNode(this, childAabb);

//BACK-RIGHT
	childCenterPos.Set(octreeCenter.x + childCenterSize.x, octreeCenter.y - childCenterSize.y, octreeCenter.z + childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[5] = new OctreeNode(this, childAabb);

//FRONT-RIGHT
	childCenterPos.Set(octreeCenter.x + childCenterSize.x, octreeCenter.y - childCenterSize.y, octreeCenter.z - childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[6] = new OctreeNode(this, childAabb);

//FRONT-LEFT
	childCenterPos.Set(octreeCenter.x - childCenterSize.x, octreeCenter.y - childCenterSize.y, octreeCenter.z - childCenterSize.z);
	childAabb.SetFromCenterAndSize(childCenterPos, childSize);
	childs[7] = new OctreeNode(this, childAabb);
}

void OctreeNode::Reorder()
{
	auto it = objects.begin();

	while (it != objects.end())
	{
		GameObject* tmp = *it;
		AABB tmpAabb(tmp->aabb);

		bool intersections[8];

		for (int i = 0; i < 8; i++)
		{
			intersections[i] = childs[i]->aabb.Intersects(tmpAabb);
		}

		if (intersections[0] && intersections[1] && intersections[2] && intersections[3] && intersections[4] && intersections[5] && intersections[6] && intersections[7])
		{
			it++;
		}
		else
		{
			it = objects.erase(it);
			for (int i = 0; i < 8; i++)
			{
				if (intersections[i])
				{
					childs[i]->Insert(tmp);
				}
			}
		}
	}
}

template<typename TYPE>
inline void OctreeNode::CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const
{
	if (primitive.Intersects(aabb))
	{
		for (std::vector<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->aabb))
			{
				objects.push_back(*it);
			}
		}

		for (int i = 0; i < 8; ++i)
		{
			if (childs[i])
			{
				childs[i]->CollectIntersections(objects, primitive);
			}
		}
	}
}