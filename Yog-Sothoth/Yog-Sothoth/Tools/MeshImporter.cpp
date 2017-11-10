#include "MeshImporter.h"
#include "../../Assimp/Assimp/include/vector3.h"
#include "../../Assimp/Assimp/include/cimport.h"
#include "../../Assimp/Assimp/include/postprocess.h"
#include "../Application.h"
#include "../Engine/Resources/R_Mesh.h"
#include "Static/JsonSerializer.h"

bool MeshImporter::Import(const aiScene* scene)
{
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* ai_mesh = scene->mMeshes[i];
		R_Mesh* mesh = new R_Mesh();

		mesh->vertices.numVertices = ai_mesh->mNumVertices;
		mesh->vertices.vertices = new float[mesh->vertices.numVertices * 3];

		memcpy(mesh->vertices.vertices, ai_mesh->mVertices, sizeof(float)*  mesh->vertices.numVertices * 3);
		//Normals
		if (ai_mesh->HasNormals())
		{
			mesh->normals.numNormals = ai_mesh->mNumVertices;
			mesh->normals.normals = new float[mesh->normals.numNormals * 3];

			memcpy(mesh->normals.normals, ai_mesh->mNormals, sizeof(float) * mesh->normals.numNormals * 3);
		}

		if (ai_mesh->HasFaces())
		{
			yogConsole(CONSOLE_INFO, "New mesh with %d vertices", mesh->vertices.numVertices);

			mesh->indices.numIndices = ai_mesh->mNumFaces * 3;
			mesh->indices.indices = new uint[mesh->indices.numIndices]; // Asume all are triangles
			for (uint j = 0; j < ai_mesh->mNumFaces; j++)
			{
				if (ai_mesh->mFaces[j].mNumIndices != 3)
				{
					yogConsole(CONSOLE_WARNING, "geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&mesh->indices.indices[j * 3], ai_mesh->mFaces[j].mIndices, sizeof(uint) * 3);
				}
			}
		}
		if (ai_mesh->HasTextureCoords(0))
		{
			mesh->uv.uv = new float[mesh->vertices.numVertices * 2];
			aiVector3D* tmp = ai_mesh->mTextureCoords[0];
			for (uint i = 0; i < mesh->vertices.numVertices * 2; i += 2)
			{
				mesh->uv.uv[i] = tmp->x;
				mesh->uv.uv[i + 1] = tmp->y;
				++tmp;
			}
		}
		mesh->SaveMeshFile();

		std::string output;
		std::string meshName;
		meshName.append(std::to_string(mesh->uid));
		meshName.append(".meta");
		JsonSerializer::Serialize(mesh, output, meshName);
	}
	return true;
}
