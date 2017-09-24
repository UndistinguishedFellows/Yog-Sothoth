#pragma once

#include "../MathGeoLib/MathGeoLib.h"

class YogTest
{
public:
	YogTest();
	~YogTest();

	//TEST - MathGeoLib
	bool SpheresIntertionTest(float3 s1Pos,float s1Rad, float3 s2Pos, float s2Rad);
};