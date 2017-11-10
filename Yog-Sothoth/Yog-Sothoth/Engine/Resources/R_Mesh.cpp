#include "R_Mesh.h"
#include <fstream>

//##########################################################
//	Binary Mesh File order:
//		- Uint control number: 1 -> mesh file
//		- Uint number of Indices
//		- Uint number of Vertices
//		- Uint number of Normals
//		- Uint number of UV
//		- Uint* Indices
//		- Float* Vertices
//		- Float* Normals
//		- Float* UVs
//##########################################################

R_Mesh::R_Mesh()
{
}

R_Mesh::~R_Mesh()
{
	RELEASE(indices.indices);
}

void R_Mesh::SaveMeshFile()
{
	LCG lcg;
	uid = lcg.Int();

	std::string filename = std::to_string(uid);
	filename.append(".mesh");

	unsigned int header[5] = {type, indices.numIndices, vertices.numVertices, normals.numNormals, uv.numUV};

	unsigned int size = sizeof(unsigned int) * 5 + sizeof(float) * indices.numIndices + sizeof(float) * vertices.numVertices * 3 + sizeof(float) * normals.numNormals * 3 + sizeof(float) * uv.numUV * 2;
	char* data = new char[size];
	char* cursor = data;

	unsigned int bytes = sizeof(header);
	memcpy(cursor, header, bytes);
	cursor += bytes;

	bytes = sizeof(unsigned int) * indices.numIndices;
	memcpy(cursor, indices.indices, bytes);
	cursor += bytes;

	bytes = sizeof(float) * vertices.numVertices * 3;
	memcpy(cursor, vertices.vertices, bytes);
	cursor += bytes;

	bytes = sizeof(float) * normals.numNormals * 3;
	memcpy(cursor, normals.normals, bytes);
	cursor += bytes;

	bytes = sizeof(float) * uv.numUV * 2;
	memcpy(cursor, uv.uv, bytes);

	std::ofstream file(filename, std::ofstream::out | std::ios::binary);

	file << data;

	file.close();
}

void R_Mesh::LoadMeshFile(const char* filename)
{
	std::ifstream file(filename, std::ios::binary);

	char* buffer = nullptr;
	file.read(buffer, file.tellg());
	char* cursor = buffer;

	unsigned int header[5];
	unsigned int bytes = sizeof(header);

	memcpy(header, cursor, bytes);
	cursor += bytes;

	type = header[0];
	indices.numIndices = header[1];
	vertices.numVertices = header[2];
	normals.numNormals = header[3];
	uv.numUV = header[4];

	bytes = sizeof(unsigned int) * indices.numIndices;
	memcpy(indices.indices, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * vertices.numVertices * 3;
	memcpy(vertices.vertices, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * normals.numNormals *3;
	memcpy(normals.normals, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * uv.numUV * 2;
	memcpy(uv.uv, cursor, bytes);
}

//Metadata UUID.meta
// fbx from name
// mesh name
// save data

void R_Mesh::Serialize(Json::Value& root)
{
}

void R_Mesh::Deserialize(Json::Value& root)
{
}