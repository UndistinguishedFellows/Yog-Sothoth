#include "R_Mesh.h"

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
}
void R_Mesh::Serialize(Json::Value& root)
{

}
void R_Mesh::Deserialize(Json::Value& root)
{
}




