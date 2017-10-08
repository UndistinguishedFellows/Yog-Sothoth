#include "TextureImporter.h"
#include "../Globals.h"

TextureImporter::TextureImporter()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

TextureImporter::~TextureImporter()
{
	ilShutDown();
}
/*
GLuint TextureImporter::loadTextureTuto(const char* path)
{
	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(path);

	if (success)
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		if (!success)
		{
			error = ilGetError();
			yogConsole(CONSOLE_ERROR, "Texture conversion failed - %s - %s", error, iluErrorString(error));
			exit(-1);
		}

		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}
	else
	{
		error = ilGetError();
		yogConsole(CONSOLE_ERROR, "Texture load failed - %s - %s", error, iluErrorString(error));
		exit(-1);
	}
	ilDeleteImages(1, &imageID);
	yogConsole(CONSOLE_INFO, "Texture creation successful.");

	return textureID;
}
*/
unsigned int TextureImporter::loadTexture(const char* path)
{
	char tmp_path[128];
	strcpy_s(tmp_path, path);
	uint ImgID = ilutGLLoadImage(tmp_path);
	uint ret;
	if (ImgID != 0)
	{
		ret = ImgID;
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error loading texture %s", path);
		ret = 0;
	}

	return ret;
}