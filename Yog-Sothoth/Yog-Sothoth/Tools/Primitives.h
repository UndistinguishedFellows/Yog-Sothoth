#pragma once
#include "../Globals.h"
#include "Interfaces/IJsonSerializable.h"

namespace Primitives
{
	struct P_Indices
	{
		uint idIndices = 0;
		uint numIndices = 0;
		uint* indices = nullptr;
	};
	struct P_Vertices
	{
		uint idVertices = 0;
		uint numVertices = 0;
		float* vertices = nullptr;
	};
	struct P_Normals
	{
		uint idNormals = 0;
		uint numNormals = 0;
		float* normals = nullptr;
	};
	struct P_UV
	{
		uint idUV = 0;
		uint numUV = 0;
		float* uv = nullptr;
	};

	struct P_Cube
	{
		P_Indices indices;
		P_Vertices vertices;
		P_Normals normals;
		P_UV uv;
		uint VAO;
	};
	struct P_Plane
	{
		P_Indices indices;
		P_Vertices vertices;
		P_Normals normals;
		P_UV uv;
		uint VAO;
	};
	struct P_Pyramid
	{
		P_Indices indices;
		P_Vertices vertices;
		P_Normals normals;
		P_UV uv;
		uint VAO;
	};
	struct P_Cilinder
	{
		P_Indices indices;
		P_Vertices vertices;
		P_Normals normals;
		P_UV uv;
		uint VAO;
	};
	struct P_Sphere
	{
		P_Indices indices;
		P_Vertices vertices;
		P_Normals normals;
		P_UV uv;
		uint VAO;
	};

	class Primitives : public IJsonSerializable
	{
	public:
		Primitives();
		~Primitives();

		P_Cube		 pCube;
		P_Plane		 pPlane;
		P_Pyramid	 pPyramid;
		P_Cilinder	 pCilinder;
		P_Sphere	 pSphere;


		void Serialize(Json::Value& root) override;
		void Deserialize(Json::Value& root) override;

	};

}
