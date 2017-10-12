#pragma once

#include "BaseClass/Module.h"
#include "../../Tools/TextureImporter.h"
#include "../../Tools/Containers/Color.h"
#include <glew.h>


#define CHECKERS_WIDTH 256
#define CHECKERS_HEIGHT 256

class Shader;

struct GPUInfo
{
	uint vendor;
	uint deviceId;
	char* brand;
	float videoMemBudget;
	float videoMemUsage;
	float videoMemAvaliable;
	float videoMemReserved;
};

class M_Renderer : public Module
{
public:
	SDL_GLContext context = nullptr;

	Json::Value root;

	bool vSync = false;
	bool fbxViewer = true;
	GPUInfo gpuInfo;

private:
	bool depthTest = false;
	bool cullFace = false;
	bool lightning = false;
	bool wireframe = false;
	bool grid = false;
	Color colorMaterial = Color(1, 1, 1, 1);

public:
	M_Renderer(bool enabled = true);
	~M_Renderer();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	bool IsVSyncActive()const;
	bool IsFBXViewerActive()const;
	void setFBXViewer(bool set);
	void SetVSync(bool set);

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig() override;
	void SaveConfig() override;

	void setGPUInfo();

	void SetDepthTest(bool stat);
	void SetCullFace(bool stat);
	void SetLightning(bool stat);
	void SetWireframe(bool stat);
	void SetGrid(bool stat);
	void SetColorMaterial(Color color);

	bool GetDepthTest();
	bool GetCullFace();
	bool GetLightning();
	bool GetWireframe();
	bool GetGrid();
	Color GetColorMaterial();

	//TEMP
	void drawCubeDirectModeTexCoord();
	void drawCubeDirectModeTexture();
	void createCheckersTexture();

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint checkTexture = 0;
	bool checkersCube = false;
	TextureImporter* texImporter = nullptr;
	unsigned int textureLenna = 0;
	
	Shader* lightShader = nullptr;		//Shader with light
	Shader* basicShader = nullptr;
	Shader* lampShader = nullptr;			//Shader of a light emmiter
	Shader* wireframeShader = nullptr;	//Shader for a wireframe display

	//Implied in refactor
	//C_Camera* frustum;
};
