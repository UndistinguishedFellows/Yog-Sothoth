#include "JsonSerializer.h"
#include "../Interfaces/IJsonSerializable.h"
#include "../../Engine/CoreModules/M_FileSystem.h"

#include <json.h>


bool JsonSerializer::Serialize(IJsonSerializable * pObj, std::string & output, std::string path)
{
	if (pObj == nullptr)
		return false;

	Json::Value serializeRoot;
	pObj->Serialize(serializeRoot);

	Json::StyledWriter writer;
	output = writer.write(serializeRoot);
	App->fs->save(path.c_str(), output.c_str(), output.size());

	return true;
}
bool JsonSerializer::DeserializeFormInput(IJsonSerializable * pObj, std::string & input)
{
	if (pObj == nullptr)
		return false;

	Json::Value deserializeRoot;
	Json::Reader reader;

	if (!reader.parse(input, deserializeRoot))
		return false;

	pObj->Deserialize(deserializeRoot);

	return true;
}
bool JsonSerializer::DeserializeFormPath(IJsonSerializable * pObj, std::string path)
{
	if (pObj == nullptr)
		return false;

	char* buffer = nullptr;
	if (App->fs->Load(path.c_str(), &buffer) > 0)
	{
		std::string input(buffer);
		RELEASE_ARRAY(buffer);
		Json::Value deserializeRoot;
		Json::Reader reader;

		if (!reader.parse(input, deserializeRoot))
			return false;

		pObj->Deserialize(deserializeRoot);
	}
	return true;
}