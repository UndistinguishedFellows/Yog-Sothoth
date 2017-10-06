#pragma once

#include "BaseClass/Module.h"
#include "../../Tools/Containers/Shader.h"
#include "../../Tools/Primitive.h"
#include "../GameObjects/Components/C_Camera.h"
#include "../GameObjects/Components/C_Mesh.h"
#include "../GameObjects/Components/C_Transform.h"
#include "../../Tools/GPUDetect/DeviceId.h"

#define CHECKERS_WIDTH 256
#define CHECKERS_HEIGHT 256

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
	SDL_GLContext context;

	Json::Value root;

	bool vSync;

	GPUInfo gpuInfo;

private:
	bool depthTest;
	bool cullFace;
	bool lightning;
	bool wireframe;
	bool grid;
	Color colorMaterial;

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
	void createCheckersTexture();

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	GLuint ImageName;
	bool checkersCube;

	unsigned int VBO, VAO, EBO, cubeVAO, cubeVBO;
	unsigned int lightVAO;
	unsigned int shaderProgram;
	C_Camera* frustum;
	std::vector<C_Mesh*> meshes;
	GameObject* testCube;
	GameObject* testLight;
	
	Shader* lightShader;
	Shader* basicShader;
	Shader* lampShader;
	Shader* wireframeShader;
};
