#pragma once
#include "../../../MathGeoLib/Math/float3.h"
#include "BaseClass/Component.h"

class C_Light : public Component
{
public:
	float3 ambient;
	float3 diffuse;
	float3 specular;

public:
	C_Light(GameObject* parent, 
			const float3& ambient = float3(1.f, 1.f, 1.f),
			const float3& diffuse = float3(1.f, 1.f, 1.f),
			const float3& specular = float3(1.f, 1.f, 1.f))
		: Component(parent),
		  ambient(ambient),
		  diffuse(diffuse),
		  specular(specular)
	{
		type = C_LIGHT;
	}
	~C_Light();

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
};
