#pragma once
#include "../../Application.h"
#include "../../Globals.h"
#include <string>

class IJsonSerializable;

class JsonSerializer
{
public:
	static bool Serialize(IJsonSerializable* pObj, std::string& output, std::string path);
	static bool DeserializeFormInput(IJsonSerializable* pObj, std::string& input);
	static bool DeserializeFormPath(IJsonSerializable* pObj, std::string path);

private:
	JsonSerializer();
};