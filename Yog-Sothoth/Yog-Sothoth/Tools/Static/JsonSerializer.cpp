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
	std::ifstream in(path.c_str());
	// file size
	in.seekg(0, std::ios::end);
	int length = in.tellg();
	in.seekg(0, in.beg);
	// allocate memory for buffer
	buffer = new char[length];
	in.read(buffer, length);
	in.close();

	if (length > 0)
	{
		std::string input(buffer);		
		Json::Value deserializeRoot;
		Json::Reader reader;

		if (!reader.parse(input, deserializeRoot))
			return false;

		pObj->Deserialize(deserializeRoot);
	}
	RELEASE_ARRAY(buffer);
	return true;
}