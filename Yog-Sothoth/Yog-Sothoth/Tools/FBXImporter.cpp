#include "FBXImporter.h"
#include <SDL.h>
#include "../Engine/CoreModules/M_Window.h"

#include "..\Assimp\Assimp\include\scene.h"
#include "..\Assimp\Assimp\include\cfileio.h"
#include "..\Assimp\Assimp\include\cimport.h"
#include "..\Assimp\Assimp\include\postprocess.h"

#include "../../OpenGL.h"



FBXImporter::FBXImporter()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);


}


FBXImporter::~FBXImporter()
{
	aiDetachAllLogStreams();
}

void FBXImporter::LoadFBX(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		// use scene->mNumMeshes to iterate on scene->mMeshes array
		//meshes = new VramVertex[scene->mNumMeshes];

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//Todo
			VramVertex* mesh = new VramVertex();
			aiMesh* new_mesh = scene->mMeshes[i];

			mesh->numVertices = new_mesh->mNumVertices;
			mesh->vertices = new float[mesh->numVertices * 3];

			memcpy(mesh->vertices, new_mesh->mVertices, sizeof(float)*  mesh->numVertices * 3);

			//Normals
			if (new_mesh->HasNormals())
			{
				mesh->numNormals = new_mesh->mNumVertices;
				mesh->normals = new float[mesh->numNormals * 3];

				memcpy(mesh->normals, new_mesh->mNormals, sizeof(float) * mesh->numNormals * 3);
			}

			if (new_mesh->HasFaces())
			{
				SDL_Log("New mesh with %d vertices", mesh->numVertices);

				if (scene->mMeshes[i]->mNumFaces * 3)
				{
					mesh->numIndices = scene->mMeshes[i]->mNumFaces * 3;
					mesh->indices = new uint[mesh->numIndices]; // Asume all are triangles
					for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
					{
						if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
						{
							SDL_Log("WARNING, geometry face with != 3 indices!");
						}
						else
						{
							memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
						}
					}
				}
			}

			meshes.push_back(mesh);

			//Generating GL Buffers
			glGenVertexArrays(1, &meshes[i]->VAO);
			glBindVertexArray(meshes[i]->VAO);
			//vertices
			glGenBuffers(1, (GLuint*) &(meshes[i]->idVertices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idVertices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*meshes[i]->numVertices * 3, meshes[i]->vertices, GL_STATIC_DRAW);

			//normals
			if (mesh->normals != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(meshes[i]->idNormals));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idNormals);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*meshes[i]->numNormals * 3, meshes[i]->normals, GL_STATIC_DRAW);
			}
			//indices
			glGenBuffers(1, (GLuint*) &(meshes[i]->idIndices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idIndices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*meshes[i]->numIndices, meshes[i]->indices, GL_STATIC_DRAW);

			//UV
			if (mesh->UV != nullptr)
			{
				glGenBuffers(1, (GLuint*) &(meshes[i]->idUV));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idUV);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*meshes[i]->numVertices * 3, meshes[i]->UV, GL_STATIC_DRAW);
			}

		}
		aiReleaseImport(scene);
	}
	else
		SDL_Log("Error loading Scene %s", path);

}

void FBXImporter::DrawMeshes(std::vector<VramVertex*> drawMeshes)
{
	for (uint i = 0; i < drawMeshes.size(); i++)
	{
		VramVertex* m = drawMeshes.at(i);

//		glEnableClientState(GL_VERTEX_ARRAY);
//		glEnableClientState(GL_NORMAL_ARRAY);
//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//glBindTexture(GL_TEXTURE_2D, id);

		//vertices
		glBindBuffer(GL_ARRAY_BUFFER, m->idVertices);
//
//		//normals
		glBindBuffer(GL_ARRAY_BUFFER, m->idNormals);
//
		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->idIndices);

		//UV
		glBindBuffer(GL_ARRAY_BUFFER, m->idUV);
//		if (m->idUV != 0)
//		{
//			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
//		}

		glBindVertexArray(m->VAO);

		//glColor4f(0.7f, 0.7f, 0.7f, 1.f);

		glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, NULL);

		//glBindTexture(GL_TEXTURE_2D, 0);
//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//		glDisableClientState(GL_NORMAL_ARRAY);
//		glDisableClientState(GL_VERTEX_ARRAY);

	}
}