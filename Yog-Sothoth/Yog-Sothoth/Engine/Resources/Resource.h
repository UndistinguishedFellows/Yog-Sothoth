#pragma once

enum rType
{
	R_UNKNOWN,
	R_MESH,
	R_MATERIAL
};

class Resource
{
public:
	rType type;
	uint used;

	explicit Resource(rType type)
		: type(type)
	{
		used = 0;
	}
	virtual ~Resource()
	{
		
	}

};