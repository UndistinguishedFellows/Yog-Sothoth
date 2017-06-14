#pragma once
#include "../../Globals.h"
#include "../../Application.h"
#include "BaseClass/Module.h"



class M_FileSystem : public Module
{
public:
	M_FileSystem(bool enable = true);
	~M_FileSystem();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	bool addPath(const char* pathOrZip, const char* mountPoint = nullptr);
	bool exist(const char* file);
	bool isDirectory(const char* file);
	bool makeDirectory(const char* dir, const char* mount = nullptr) const;
	const char* getSaveLocalDirectory() { return ROOT; }

	const char* getBasePath();

	unsigned int load(const char* file, char** buffer)const;
	SDL_RWops* load(const char* file)const;
	unsigned int save(const char* file, const char* buffer, unsigned int size);
};
