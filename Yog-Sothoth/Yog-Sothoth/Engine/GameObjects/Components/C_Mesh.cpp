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
		uv.uv = new float[vertices.numVertices * 2];
		aiVector3D* tmp = mesh->mTextureCoords[0];
		for (uint i = 0; i < vertices.numVertices * 2; i += 2)
		{
			uv.uv[i] = tmp->x;
			uv.uv[i + 1] = tmp->y;
			++tmp;
		}
	}

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
//		for (int i = 0; i < vertices.numVertices; i += 2)
//		{
//			yogConsole(CONSOLE_INFO, "UV: (%f, %f)", uv.uv[i], uv.uv[i+1]);
//		}
		

	}
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void C_Mesh::Draw(Shader shader, C_Camera* camera) const
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float4x4 view = camera->camera.ViewMatrix();
	C_Transform* model = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	C_Transform* trans = (C_Transform*)App->renderer->testLight->FindComponent(C_TRANSFORM);

	shader.Use();
	shader.setInt("tex", 0);
	if (associatedMaterial != nullptr)
	{
		if(associatedMaterial->texture != 0)
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
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
							//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.idIndices);
	
	shader.setMat4("projection", &camera->camera.ProjectionMatrix().Transposed());
	shader.setMat4("view", &view.Transposed());
	//shader.setMat4("model", &float4x4::identity);
	shader.setMat4("model", &model->globalTransform.Transposed());
	shader.setVec3("objectColor", color.r, color.g, color.b);
	shader.setVec3("lightColor", 0.50f, 0.50f, 0.50f);
	shader.setVec3("lightPos", &trans->GetPosition());
	shader.setVec3("viewPos", &camera->camera.pos);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, associatedMaterial->texture);

	glDrawElements(GL_TRIANGLES, indices.numIndices, GL_UNSIGNED_INT, 0);		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void C_Mesh::DrawNormals(Shader shader, C_Camera* camera) const
{
	C_Transform* trans = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	float4x4 view = camera->camera.ViewMatrix();

	shader.Use();
	shader.setMat4("projection", &camera->camera.ProjectionMatrix().Transposed());
	shader.setMat4("view", &view.Transposed());
	shader.setMat4("model", &trans->globalTransform.Transposed());
	shader.setVec4("objectColor", 1.f, 1.f, 0.f, 1.f);

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	for (uint i = 0; i < vertices.numVertices; ++i)
	{
		float3 vertice(&vertices.vertices[i * 3]);
		float3 normal(&normals.normals[i * 3]);
		float3 e_normal(vertice + (normal*0.4f));

		glVertex3f(vertice.x, vertice.y, vertice.z);
		glVertex3f(e_normal.x, e_normal.y, e_normal.z);
	}
	glEnd();
	glLineWidth(1.0f);
	glUseProgram(0);

}

void C_Mesh::DrawSelected(Shader shader, C_Camera* camera) const
{
}

void C_Mesh::UpdateBoundingBoxes()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.vertices, vertices.numVertices);

	obb = aabb;
	C_Transform* transform = (C_Transform*)parent->FindComponent(C_TRANSFORM);
	obb.Transform(transform->globalTransform);
	aabb.SetFrom(obb);

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
