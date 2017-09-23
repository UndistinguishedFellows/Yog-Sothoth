#include "YogTest.h"
#include "../MathGeoLib/MathGeoLib.h"

YogTest::YogTest()
{
}

YogTest::~YogTest()
{
}

bool YogTest::SpheresIntertionTest(float3 s1Pos, float s1Rad, float3 s2Pos, float s2Rad)
{
	bool intersects = false;

	Sphere s1 = Sphere(s1Pos, s1Rad);
	Sphere s2 = Sphere(s2Pos, s2Rad);

	intersects = s1.Intersects(s2);

	return intersects;
}