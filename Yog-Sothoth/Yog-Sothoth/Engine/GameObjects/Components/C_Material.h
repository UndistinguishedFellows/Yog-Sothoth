#pragma once
#include "../../../MathGeoLib/Math/float3.h"
#include "BaseClass/Component.h"
#include "../../CoreModules/M_Window.h"
#include "../../../Tools/Containers/Color.h"

class C_Material : public Component
{
public:
	float3 ambient;
	float3 diffuse;
	float3 specular;
	float shininess;

	Color color;

	uint texture;

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
	}

	~C_Material() override;

	uint LoadTexture(const char* path);

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};
