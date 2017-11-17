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

	explicit Resource(rType type)
		: type(type)
	{
	}
	virtual ~Resource()
	{
		
	}

};