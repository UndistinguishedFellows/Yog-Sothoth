#include "C_Mesh.h"
#include "../GameObject.h"
#include <glew.h>
#include "../../../../Assimp/Assimp/include/scene.h"
#include "../../../../Assimp/Assimp/include/cfileio.h"
#include "../../../../Assimp/Assimp/include/cimport.h"
#include "../../../../Assimp/Assimp/include/postprocess.h"
#include "C_Transform.h"
#include "../../../Application.h"


C_Mesh::C_Mesh(GameObject* parent) : Component(parent)
{

}

C_Mesh::~C_Mesh()
{
	//Delete arrays
	if (vertices.vertices != nullptr)
	{
		delete vertices.vertices;
	}
	if (indices.indices != nullptr)
	{
		delete indices.indices;
	}
	if (normals.normals != nullptr)
	{
		delete normals.normals;
	}
	if (uv.uv != nullptr)
	{
		delete uv.uv;
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

void C_Mesh::Load(const aiMesh* mesh)
{
	vertices.numVertices = mesh->mNumVertices;
	vertices.vertices = new float[vertices.numVertices * 3];

	memcpy(vertices.vertices, mesh->mVertices, sizeof(float)*  vertices.numVertices * 3);
	//Normals
	if (mesh->HasNormals())
	{
		normals.numNormals = mesh->mNumVertices;
		normals.normals = new float[normals.numNormals * 3];

		memcpy(normals.normals, mesh->mNormals, sizeof(float) * normals.numNormals * 3);
	}

	if (mesh->HasFaces())
	{
		yogConsole(CONSOLE_INFO, "New mesh with %d vertices", vertices.numVertices);

		indices.numIndices = mesh->mNumFaces * 3;
		indices.indices = new uint[indices.numIndices]; // Asume all are triangles
		for (uint j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices != 3)
			{
				yogConsole(CONSOLE_WARNING, "geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&indices.indices[j * 3], mesh->mFaces[j].mIndices, sizeof(uint) * 3);
			}
		}
	}
	if (mesh->HasTextureCoords(0))
	{
		uv.uv = new float[vertices.numVertices * 3];
		memcpy(uv.uv, mesh->mTextureCoords[0], sizeof(float) * vertices.numVertices * 3);
	}

	//Generating GL Buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, (GLuint*) &(vertices.idVertices));
	glGenBuffers(1, (GLuint*) &(indices.idIndices));

	glBindVertexArray(VAO);

	//vertices	
	glBindBuffer(GL_ARRAY_BUFFER, vertices.idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.numVertices * 3, vertices.vertices, GL_STATIC_DRAW);
	//indices	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.numIndices, indices.indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

//	//normals
//	if (normals.normals != nullptr)
//	{
//		glGenBuffers(1, (GLuint*) &(normals.idNormals));
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normals.idNormals);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*normals.numNormals * 3, normals.normals, GL_STATIC_DRAW);
//	}
//	//UV
//	if (uv.uv != nullptr)
//	{
//		glGenBuffers(1, (GLuint*) &(uv.idUV));
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uv.idUV);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*vertices.numVertices * 3, uv.uv, GL_STATIC_DRAW);
//	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void C_Mesh::Draw(Shader shader, C_Camera* camera) const
{
	glUseProgram(shader.shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
							//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.idIndices);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, camera->camera.ProjectionMatrix().Transposed().ptr());
	math::float4x4 view = camera->camera.ViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, view.Transposed().ptr());
	C_Transform* model = (C_Transform*)parent->FindComponent(TRANSFORM);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, model->globalTransform.Transposed().ptr());
	glUniform3f(glGetUniformLocation(shader.shaderProgram, "objectColor"), color.r, color.g, color.b);
	glUniform3f(glGetUniformLocation(shader.shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(shader.shaderProgram, "lightPos"), 1, (const GLfloat*)&App->renderer->lightPos);
	glUniform3fv(glGetUniformLocation(shader.shaderProgram, "viewPos"), 1, camera->camera.pos.ptr());
	//glUniform4fv(glGetUniformLocation(shader.shaderProgram, "color"), 1, (const GLfloat*)&color);
	glDrawElements(GL_TRIANGLES, indices.numIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void C_Mesh::UpdateBoundingBoxes()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.vertices, vertices.numVertices);

	obb = aabb;
	C_Transform* transform = (C_Transform*)parent->FindComponent(TRANSFORM);
	obb.Transform(transform->globalTransform);
	aabb.SetFrom(obb);

}

void C_Mesh::Serialize(Json::Value& root)
{
}

void C_Mesh::Deserialize(Json::Value& root)
{
}
