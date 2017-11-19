#pragma once
#include "../../Globals.h"
#include "../../../MathGeoLib/MathGeoLib.h"
#include "BaseClass/Component.h"
#include "../../CoreModules/M_Window.h"
#include "../../../Tools/Containers/Color.h"
#include "../../../Tools/TextureImporter.h"
#include "../../Resources/R_Material.h"

class C_Material : public Component
{
public:

	bool checkers = false;

	float3 ambient;
	float3 diffuse;
	float3 specular;
	float shininess;

	Color color;

	//uint texture;

	ImageInfo imInfo;

	R_Material* rMaterial = nullptr;

	C_Material(GameObject* parent,
			   const float3& ambient = float3(1.f, 1.f, 1.f),
			   const float3& diffuse = float3(1.f, 1.f, 1.f),
			   const float3& specular = float3(1.f, 1.f, 1.f),
			   float shininess = 1.f)
		: Component(parent),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shininess(shininess)
	{
		type = C_MATERIAL;
	}

	~C_Material() override;

	uint LoadTexture(const char* path);

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};
