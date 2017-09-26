#pragma once

#include "BaseClass/Module.h"
#include "../../Tools/Containers/Shader.h"
#include "../../Tools/Primitive.h"
#include "../GameObjects/Components/C_Camera.h"
#include "../GameObjects/Components/C_Mesh.h"

class M_Renderer : public Module
{
public:
	SDL_GLContext context;

	Json::Value root;

	bool vSync;
private:
	bool depthTest;
	bool cullFace;
	bool lightning;
	bool wireframe;
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

	void SetDepthTest(bool stat);
	void SetCullFace(bool stat);
	void SetLightning(bool stat);
	void SetWireframe(bool stat);
	void SetColorMaterial(Color color);
	bool GetDepthTest();
	bool GetCullFace();
	bool GetLightning();
	bool GetWireframe();
	Color GetColorMaterial();


	//TEMP
	unsigned int VBO, VAO, EBO;
	unsigned int shaderProgram;
	C_Camera* frustum;
	std::vector<C_Mesh*> meshes;
	
	Shader basicShader;
};
