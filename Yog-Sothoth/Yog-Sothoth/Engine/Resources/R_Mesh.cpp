#include "R_Mesh.h"
#include <fstream>
#include "../../../Assimp/Assimp/include/vector3.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include <glew.h>
#include "../../MathGeoLib/Algorithm/Random/LCG.h"

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

R_Mesh::R_Mesh() : Resource(R_MESH)
{
}

R_Mesh::~R_Mesh()
{
	if (vertices.vertices != nullptr)
	{
		RELEASE(vertices.vertices);
	}
	if (indices.indices != nullptr)
	{
		RELEASE(indices.indices);
	}
	if (normals.normals != nullptr)
	{
		RELEASE(normals.normals);
	}
	if (uv.uv != nullptr)
	{
		RELEASE(uv.uv);
	}

	// Delete Buffers and restore de ID's
	if (vertices.idVertices != 0)
	{
		glDeleteBuffers(1, &vertices.idVertices);
		vertices.idVertices = 0;
	}
	if (indices.idIndices != 0)
	{
		glDeleteBuffers(1, &indices.idIndices);
		indices.idIndices = 0;
	}
	if (normals.idNormals != 0)
	{
		glDeleteBuffers(1, &normals.idNormals);
		normals.idNormals = 0;
	}
	if (uv.idUV != 0)
	{
		glDeleteBuffers(1, &uv.idUV);
		uv.idUV = 0;
	}
}

void R_Mesh::SaveMeshFile()
{
	LCG lcg;
	uuid = lcg.Int();

	std::string filename = "data/library/";
	filename.append(std::to_string(uuid));
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
	if (uv.numUV > 0)
	{
		bytes = sizeof(float) * uv.numUV * 2;
		memcpy(cursor, uv.uv, bytes);

	}

	std::ofstream file(filename, std::ofstream::out | std::ios::binary);

	file.write(data, size);	

	file.close();
	if (size > 0)
		RELEASE_ARRAY(data)
}

void R_Mesh::LoadMeshFile(const char* filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);

	char* buffer = nullptr;

	// file size
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	buffer = new char[length];
	file.read(buffer, length);

	unsigned int header[5];
	unsigned int bytes = sizeof(header);

	char* cursor = buffer;

	memcpy(header, cursor, bytes);
	cursor += bytes;

	type = (rType)header[0];
	indices.numIndices = header[1];
	vertices.numVertices = header[2];
	normals.numNormals = header[3];
	uv.numUV = header[4];

	bytes = sizeof(unsigned int) * indices.numIndices;
	indices.indices = new unsigned int[indices.numIndices];
	memcpy(indices.indices, cursor, bytes);
	cursor += bytes;

	bytes = sizeof(float) * vertices.numVertices * 3;
	vertices.vertices = new float[vertices.numVertices * 3];
	memcpy(vertices.vertices, cursor, bytes);
	cursor += bytes;

	if (normals.numNormals > 0)
	{
		bytes = sizeof(float) * normals.numNormals * 3;
		normals.normals = new float[normals.numNormals * 3];
		memcpy(normals.normals, cursor, bytes);
		cursor += bytes;
	}

	if (uv.numUV > 0)
	{
		bytes = sizeof(float) * uv.numUV * 2;
		uv.uv = new float[uv.numUV * 2];
		memcpy(uv.uv, cursor, bytes);

	}

	RELEASE(buffer);

	GenBuffers();
}

void R_Mesh::Load(const aiMesh* aiMesh)
{
	vertices.numVertices = aiMesh->mNumVertices;
	vertices.vertices = new float[vertices.numVertices * 3];

	memcpy(vertices.vertices, aiMesh->mVertices, sizeof(float)*  vertices.numVertices * 3);
	//Normals
	if (aiMesh->HasNormals())
	{
		normals.numNormals = aiMesh->mNumVertices;
		normals.normals = new float[normals.numNormals * 3];

		memcpy(normals.normals, aiMesh->mNormals, sizeof(float) * normals.numNormals * 3);
	}

	if (aiMesh->HasFaces())
	{
		yogConsole(CONSOLE_INFO, "New mesh with %d vertices", vertices.numVertices);

		indices.numIndices = aiMesh->mNumFaces * 3;
		indices.indices = new uint[indices.numIndices]; // Asume all are triangles
		for (uint j = 0; j < aiMesh->mNumFaces; j++)
		{
			if (aiMesh->mFaces[j].mNumIndices != 3)
			{
				yogConsole(CONSOLE_WARNING, "geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&indices.indices[j * 3], aiMesh->mFaces[j].mIndices, sizeof(uint) * 3);
			}
		}
	}
	if (aiMesh->HasTextureCoords(0))
	{
		uv.uv = new float[vertices.numVertices * 2];
		uv.numUV = vertices.numVertices;
		aiVector3D* tmp = aiMesh->mTextureCoords[0];
		for (uint i = 0; i < vertices.numVertices * 2; i += 2)
		{
			uv.uv[i] = tmp->x;
			uv.uv[i + 1] = tmp->y;
			++tmp;
		}
	}
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

void R_Mesh::AddMeta(std::string prefab)
{
	std::ofstream outfile;
	std::string name = "data/library/";
	name.append(std::to_string(uuid));
	name.append(".meta");
	outfile.open(name, std::ios_base::app);
	outfile << "{ \"prefab\" : \"" << prefab << "\"}" << std::endl;
}

void R_Mesh::RemoveMeta(std::string prefab)
{
}

void R_Mesh::GenBuffers()
{
	//Generating GL Buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, (GLuint*) &(vertices.idVertices));
	glGenBuffers(1, (GLuint*) &(indices.idIndices));

	glBindVertexArray(VAO);

	//vertices	
	glBindBuffer(GL_ARRAY_BUFFER, vertices.idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.numVertices * 3, vertices.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//indices	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.numIndices, indices.indices, GL_STATIC_DRAW);

	//normals
	if (normals.normals != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(normals.idNormals));
		glBindBuffer(GL_ARRAY_BUFFER, normals.idNormals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.numNormals * 3, normals.normals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}
	//UV
	if (uv.uv != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(uv.idUV));
		glBindBuffer(GL_ARRAY_BUFFER, uv.idUV);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.numVertices * 2, uv.uv, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

