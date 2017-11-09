#include "R_Mesh.h"
#include <fstream>

R_Mesh::R_Mesh()
{
}

R_Mesh::~R_Mesh()
{
	RELEASE(indices.indices);
}

void R_Mesh::MakeMeshFile()
{
	LCG lcg;
	uid = lcg.Int();

	//Codigo para crear el archivo en binario que guarda la mesh
	//Mesh UUID.mesh
	//Metadatos UUID.metadata
	// FBX del que proviene
	// Nombre de la mesh
	// Fecha de la creación de la mech
	// etc

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

	std::ofstream file("test.txt", std::ofstream::out | std::ios::binary);

	file << data;

	file.close();
}

void R_Mesh::Serialize(Json::Value& root)
{
}

void R_Mesh::Deserialize(Json::Value& root)
{
}