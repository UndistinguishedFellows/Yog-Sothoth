#include "Primitives.h"
#include "Static/JsonSerializer.h"

Primitives::Primitives::Primitives()
{
	JsonSerializer::DeserializeFormPath(this, "data/assets/primitives/cube.json");
}

Primitives::Primitives::~Primitives()
{
}

void Primitives::Primitives::Serialize(Json::Value& root)
{
}

void Primitives::Primitives::Deserialize(Json::Value& root)
{
	Json::Value jpCube = root.get("pCube", 0);
	if (jpCube != 0)
	{
		pCube.vertices.numVertices = jpCube.get("numVertices", 0).asInt()*3;
		pCube.indices.numIndices = jpCube.get("numIndices", 0).asInt();
		pCube.indices.indices = new uint[pCube.indices.numIndices];
		pCube.vertices.vertices = new float[pCube.vertices.numVertices];
		Json::Value jValue;
		jValue = jpCube.get("indices", root);
		for (int i = 0; i != jValue.size(); ++i)
		{
			pCube.indices.indices[i] = jValue[i].asUInt();
		}
		jpCube = root.get("pCube", 0);
		jValue = jpCube.get("vertices", root);		

		for (int i = 0; i != jValue.size(); ++i)
		{
			pCube.vertices.vertices[i] = jValue[i].asFloat();
		}
	}
}
