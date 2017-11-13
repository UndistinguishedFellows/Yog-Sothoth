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
}

void C_Mesh::Load(const aiMesh* mesh)
{
	rMesh->vertices.numVertices = mesh->mNumVertices;
	rMesh->vertices.vertices = new float[rMesh->vertices.numVertices * 3];

	memcpy(rMesh->vertices.vertices, mesh->mVertices, sizeof(float)*  rMesh->vertices.numVertices * 3);
	//Normals
	if (mesh->HasNormals())
	{
		rMesh->normals.numNormals = mesh->mNumVertices;
		rMesh->normals.normals = new float[rMesh->normals.numNormals * 3];

		memcpy(rMesh->normals.normals, mesh->mNormals, sizeof(float) * rMesh->normals.numNormals * 3);
	}

	if (mesh->HasFaces())
	{
		yogConsole(CONSOLE_INFO, "New mesh with %d vertices", rMesh->vertices.numVertices);

		rMesh->indices.numIndices = mesh->mNumFaces * 3;
		rMesh->indices.indices = new uint[rMesh->indices.numIndices]; // Asume all are triangles
		for (uint j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices != 3)
			{
				yogConsole(CONSOLE_WARNING, "geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&rMesh->indices.indices[j * 3], mesh->mFaces[j].mIndices, sizeof(uint) * 3);
			}
		}
	}
	if (mesh->HasTextureCoords(0))
	{
		rMesh->uv.uv = new float[rMesh->vertices.numVertices * 2];
		aiVector3D* tmp = mesh->mTextureCoords[0];
		for (uint i = 0; i < rMesh->vertices.numVertices * 2; i += 2)
		{
			rMesh->uv.uv[i] = tmp->x;
			rMesh->uv.uv[i + 1] = tmp->y;
			++tmp;
		}
	}

	//Generating GL Buffers
	glGenVertexArrays(1, &rMesh->VAO);
	glGenBuffers(1, (GLuint*) &(rMesh->vertices.idVertices));
	glGenBuffers(1, (GLuint*) &(rMesh->indices.idIndices));

	glBindVertexArray(rMesh->VAO);

	//vertices	
	glBindBuffer(GL_ARRAY_BUFFER, rMesh->vertices.idVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rMesh->vertices.numVertices * 3, rMesh->vertices.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//indices	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rMesh->indices.idIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * rMesh->indices.numIndices, rMesh->indices.indices, GL_STATIC_DRAW);
	
	//normals
	if (rMesh->normals.normals != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(rMesh->normals.idNormals));
		glBindBuffer(GL_ARRAY_BUFFER, rMesh->normals.idNormals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*rMesh->normals.numNormals * 3, rMesh->normals.normals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}
	//UV
	if (rMesh->uv.uv != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(rMesh->uv.idUV));
		glBindBuffer(GL_ARRAY_BUFFER, rMesh->uv.idUV);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rMesh->vertices.numVertices * 2, rMesh->uv.uv, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);





}

void C_Mesh::Draw(Shader shader, C_Camera* camera) const
{	
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float4x4 view = camera->frustum.ViewMatrix();

	shader.Use();
	shader.setInt("tex", 0);
	if (associatedMaterial != nullptr)
	{
		if(associatedMaterial->texture != 0 || associatedMaterial->checkers)
		{
			shader.setInt("hasTex", 1);
		}
		else
			shader.setInt("hasTex", 0);
	}
	else
	{
		shader.setInt("hasTex", 0);
	}
	glBindVertexArray(rMesh->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
							//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rMesh->indices.idIndices);
	
	shader.setMat4("projection", &camera->frustum.ProjectionMatrix().Transposed());
	shader.setMat4("view", &view.Transposed());
	if (App->renderer->fbxViewer)
	{
		shader.setMat4("model", &float4x4::identity);
	}
	else
	{
		shader.setMat4("model", &ownerParent->Transform->globalTransform.Transposed());
	}
	shader.setVec3("objectColor", color.r, color.g, color.b);
	shader.setVec3("lightColor", 0.50f, 0.50f, 0.50f);
	shader.setVec3("lightPos", &App->objManager->testLight->Transform->GetPosition());
	shader.setVec3("viewPos", &camera->frustum.pos);

	glActiveTexture(GL_TEXTURE0);

	if(associatedMaterial->checkers)
	{
		glBindTexture(GL_TEXTURE_2D, App->renderer->checkTexture);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, associatedMaterial->texture);
	}

	glDrawElements(GL_TRIANGLES, rMesh->indices.numIndices, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void C_Mesh::DrawNormals(Shader shader, C_Camera* camera) const
{
	C_Transform* trans = ownerParent->Transform;
	float4x4 view = camera->frustum.ViewMatrix();

	shader.Use();
	shader.setMat4("projection", &camera->frustum.ProjectionMatrix().Transposed());
	shader.setMat4("view", &view.Transposed());
	if (App->renderer->fbxViewer)
		shader.setMat4("model", &float4x4::identity);
	else
		shader.setMat4("model", &trans->globalTransform.Transposed());
	shader.setFloat("normal_length", 0.3);

	//shader.setVec4("objectColor", 1.f, 1.f, 0.f, 1.f);

	glLineWidth(2.0f);
//	glBegin(GL_LINES);
//	for (uint i = 0; i < vertices.numVertices; ++i)
//	{
//		float3 vertice(&vertices.vertices[i * 3]);
//		float3 normal(&normals.normals[i * 3]);
//		float3 e_normal(vertice + (normal*0.4f));
//		glColor3f(normal.x,normal.x,normal.x);
//		glVertex3f(vertice.x, vertice.y, vertice.z);
//		glVertex3f(e_normal.x, e_normal.y, e_normal.z);
//	}
//	glEnd();

	glBindVertexArray(rMesh->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
							//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rMesh->indices.idIndices);

	glDrawElements(GL_TRIANGLES, rMesh->indices.numIndices, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glLineWidth(1.0f);
}

void C_Mesh::DrawSelected(Shader shader, C_Camera* camera) const
{
}

void C_Mesh::UpdateBoundingBoxes()
{
	aabb.SetNegativeInfinity();
	if (rMesh->vertices.vertices != nullptr)
	{
		aabb.Enclose((float3*)rMesh->vertices.vertices, rMesh->vertices.numVertices);

		obb = aabb;
		if(!App->renderer->fbxViewer)
			obb.Transform(ownerParent->Transform->globalTransform);
		aabb.SetFrom(obb);

	}

}

AABB C_Mesh::GetAABB()
{
	return aabb;
}

void C_Mesh::Serialize(Json::Value& root)
{
}

void C_Mesh::Deserialize(Json::Value& root)
{
}
