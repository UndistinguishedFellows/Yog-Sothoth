	
#include "M_FileSystem.h"
#include <physfs.h>
#include <string>


int closeSdlRwops(SDL_RWops *rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

M_FileSystem::M_FileSystem(bool enable) : Module(enable)
{
	yogConsole("FileSystem: Creation.");
	name.assign("filesystem");

	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	addPath(".");
}

M_FileSystem::~M_FileSystem()
{
	yogConsole("FileSystem: Destroying.");
	PHYSFS_deinit();
}

bool M_FileSystem::Init()
{
	yogConsole("FileSystem: init.");
	bool ret = true;

	std::string writeLocalPath = SDL_GetBasePath();

	if (PHYSFS_setWriteDir(writeLocalPath.c_str()) == 0)
	{
		yogConsole("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	}
	else
	{
		yogConsole("Writing directory is %s\n", writeLocalPath);
		addPath(writeLocalPath.c_str());
	}

	return ret;
}

bool M_FileSystem::Start()
{
	return true;
}

update_status M_FileSystem::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_FileSystem::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_FileSystem::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_FileSystem::CleanUp()
{
	return true;
}

bool M_FileSystem::addPath(const char* pathOrZip, const char* mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
	{
		yogConsole("File System error while adding a path or zip(%s): %s.", pathOrZip, PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

bool M_FileSystem::exist(const char* file)
{
	return PHYSFS_exists(file) != 0;
}

bool M_FileSystem::isDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

bool M_FileSystem::makeDirectory(const char* dir, const char* mount) const
{
	bool ret = false;

	if (!dir)
		return ret;

	char newDir[128];

	if (mount)
		sprintf_s(newDir, 128, "%s/%s", mount, dir);
	else
		sprintf_s(newDir, 128, dir);

	if (PHYSFS_mkdir(newDir) == 0)
	{
		yogConsole("Could not make dir: %s. PhsyFs error: %s", newDir, PHYSFS_getLastError());
	}
	else
	{
		yogConsole("Just created a dir: %s.", newDir);
		ret = true;
	}

	return ret;
}

const char* M_FileSystem::getBasePath()
{
	return PHYSFS_getBaseDir();
}

unsigned M_FileSystem::load(const char* file, char** buffer)const
{
	unsigned int ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fsFile, *buffer, 1, (PHYSFS_sint32)size);
			{
				if (readed != size)
				{
					yogConsole("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
					RELEASE(buffer);
				}
				else
				{
					ret = (uint)readed;
				}
			}
		}
		if (PHYSFS_close(fsFile) == 0)
			yogConsole("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		yogConsole("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

SDL_RWops* M_FileSystem::load(const char* file)const
{
	char* buffer;
	int size = load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != nullptr)
			r->close = closeSdlRwops;

		return r;
	}
	else
		return nullptr;
}

unsigned int M_FileSystem::save(const char * file, const char * buffer, unsigned int size)
{
	unsigned int ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openWrite(file);

	if (fsFile)
	{
		PHYSFS_sint64 written = PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
		{
			yogConsole("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)written;

		if (PHYSFS_close(fsFile) == 0)
			yogConsole("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		yogConsole("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

void M_FileSystem::Serialize(Json::Value& root)
{
}

void M_FileSystem::Deserialize(Json::Value& root)
{
}

void M_FileSystem::LoadConfig()
{
}

void M_FileSystem::SaveConfig()
{
}


