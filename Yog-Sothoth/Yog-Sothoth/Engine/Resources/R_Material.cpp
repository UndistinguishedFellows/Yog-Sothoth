#include "R_Material.h"
#include <fstream>
#include "../../MathGeoLib/Algorithm/Random/LCG.h"
#include <il.h>
#include <ilut.h>

R_Material::R_Material() : Resource(R_MATERIAL)
{
}

R_Material::~R_Material()
{
}

void R_Material::SaveMaterialFile(const char* path)
{
	LCG lcg;
	uuid = lcg.Int();
	
	/**First load the image */
	//1-Gen the image and bind it
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	char * buffer = nullptr;
	std::ifstream file;
	std::ofstream out;
	file.open(path, std::ios::in | std::ios::binary);
	// file size
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	buffer = new char[length];
	file.read(buffer, length);
	file.close();
	//2-Load the image from buffer
	if (ilLoadL(IL_TYPE_UNKNOWN, buffer, length))
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint _size;
		ILubyte* data = nullptr;

		//3-Set format (DDS, DDS compression)
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		//4-Get size
		_size = ilSaveL(IL_DDS, nullptr, 0);
		if (_size > 0)
		{
			//5-If size is more than 0 create the image buffer
			data = new ILubyte[_size];

			//6-Save the image with the correct format and save it with file system
			if (ilSaveL(IL_DDS, data, _size) > 0)
			{
				std::string name("data/library/");
				name.append(std::to_string(uuid));
				name.append(".dds");
				out.open(name, std::ios::out | std::ios::binary);
				out.write((const char*)data, _size);
				out.close();
			}

			//7-Release the image buffer to avoid memory leaks
			RELEASE_ARRAY(data);
		}

		//8-Finally if the image was loaded destroy the image to avoid more memory leaks
		ilDeleteImages(1, &image);
	}


}

void R_Material::LoadMaterialFile(const char* filename)
{
	char * buffer = nullptr;
	std::ifstream file;
	std::ofstream out;
	file.open(filename, std::ios::in | std::ios::binary);
	// file size
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	buffer = new char[length];
	file.read(buffer, length);
	file.close();

	if (buffer && length > 0)
	{
		ILuint image = 0;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoadL(IL_DDS, (const void*)buffer, length))
		{
			texture = ilutGLBindTexImage();
			ilDeleteImages(1, &image);
		}
		else
		{
			yogConsole(CONSOLE_ERROR, "Devil could not load the texture resource ... from file ... .");
		}
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Could not load texture resource ... from file ... .");
	}
	if (length > 0)
	{
		RELEASE_ARRAY(buffer);
	}
}

void R_Material::Serialize(Json::Value& root)
{
}

void R_Material::Deserialize(Json::Value& root)
{
}

void R_Material::AddMeta(std::string prefab)
{
	std::ofstream outfile;
	std::string name = "data/library/";
	name.append(std::to_string(uuid));
	name.append(".meta");
	outfile.open(name, std::ios_base::app);
	outfile << "{ \"prefab\" : \"" << prefab << "\"}" << std::endl;
}

void R_Material::RemoveMeta(std::string prefab)
{
}
