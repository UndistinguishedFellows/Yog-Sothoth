#include "../../Globals.h"
#include "../../Application.h"
#include "M_Renderer.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "../../OpenGL.h"
#include "../../Tools/Static/JsonSerializer.h"
#include "../UI_Modules/M_UIManager.h"
#include "../../../Assimp/Assimp/include/postprocess.h"
#include "../../../Assimp/Assimp/include/cimport.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include "../GameObjects/Components/C_Transform.h"

M_Renderer::M_Renderer(bool enabled) : Module(enabled)
{
	name.assign("renderer");
	frustum = new C_Camera(nullptr);
}

M_Renderer::~M_Renderer()
{
}

bool M_Renderer::Init()
{
	LoadConfig();	

	SDL_Log("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if (context == nullptr)
	{
		SDL_Log("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		glewExperimental = GL_TRUE;
		GLenum gl = glewInit();
		if (gl != GLEW_OK)
		{
			SDL_Log("REND_Error: Glew lib could not init %s\n", glewGetErrorString(gl));
			ret = false;
		}
	}
	SetVSync(vSync);
	if (ret)
	{
		// get version info
		SDL_Log("Vendor: %s", glGetString(GL_VENDOR));
		SDL_Log("Renderer: %s", glGetString(GL_RENDERER));
		SDL_Log("OpenGL version supported %s", glGetString(GL_VERSION));
		SDL_Log("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			SDL_Log("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

	
		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			SDL_Log("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			SDL_Log("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}
		SetDepthTest(depthTest);
		SetCullFace(cullFace);
	}

	setGPUInfo();

	SetWireframe(wireframe);

	return ret;
}

bool M_Renderer::Start()
{
	lightShader = new Shader("data/shaders/1.basic_lighting.vs", "data/shaders/1.basic_lighting.fs");
	basicShader = new Shader("data/shaders/camera.vs", "data/shaders/basicFragment.fs");	//basicShader loaded to render normals in direct mode ??? idk why but its necessary at the moment
	lampShader = new Shader("data/shaders/1.lamp.vs", "data/shaders/1.lamp.fs");
	
	//###########################################

	App->objManager->LoadFBX("data/assets/cube.FBX");
	testCube = App->objManager->FindGameObject("pCube1");
	C_Mesh* mesh = (C_Mesh*)testCube->FindComponent(C_MESH);
	mesh->color = { 1.0f, 0.5f, 0.31f, 1.f };

	App->objManager->LoadFBX("data/assets/cube.FBX");
	testLight = App->objManager->root->children.at(2);
	mesh = (C_Mesh*)testLight->FindComponent(C_MESH);
	mesh->color = { 1.0f, 1.f, 1.f, 1.f };
	testLight->CreateComponent(C_LIGHT);
	App->objManager->light = testLight;
	testLight->type = GO_LIGHT;
	C_Transform* trans = (C_Transform*)testLight->FindComponent(C_TRANSFORM);
	trans->SetPosition(float3(0.f, 20.0f, 5.0f));
	//C_Transform* trans = (C_Transform*)testLight->FindComponent(C_TRANSFORM);



	//App->objManager->LoadFBX("data/assets/warrior.FBX");
	//App->objManager->LoadFBX("data/assets/Street environment_V01.FBX");
	//App->objManager->LoadFBX("data/assets/baker_house/bakerHouse.FBX");
	App->objManager->LoadFBX("data/assets/MechaT.FBX");

	return true;
}

update_status M_Renderer::PreUpdate(float dt)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

update_status M_Renderer::Update(float dt)
{

	return UPDATE_CONTINUE;
}

update_status M_Renderer::PostUpdate(float dt)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float4x4 view = frustum->camera.ViewMatrix();

	App->objManager->root->Draw(*lightShader, frustum);
	if (App->objManager->light != nullptr)
	{
		App->objManager->light->DrawLight(*lampShader, frustum);
	}

	//Draw normals of a mesh
	App->objManager->root->DrawNormals(*basicShader, frustum);
	//Draw floor grid and world axis
	if (grid)
	{
		glPushMatrix();
		glMultMatrixf(frustum->camera.ProjectionMatrix().Transposed().ptr());
		glPushMatrix();
		glMultMatrixf(view.Transposed().ptr());
		P_Plane floor(0, 1, 0, 0);
		floor.color = { 0.1f, 0.1f, 0.1f , 1.f };
		floor.axis = true;
		floor.Render();
		glPopMatrix();
		glPopMatrix();

	}

	frustum->Move(dt);
	frustum->Rotate(dt);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	App->uiManager->DrawEditor();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

bool M_Renderer::CleanUp()
{
	SDL_Log("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);
	return true;
}

void M_Renderer::Serialize(Json::Value& root)
{
	root["vsync"] = vSync;
	root["depthTest"] = depthTest;
	root["cullFace"] = cullFace;
	root["wireframe"] = wireframe;
	root["grid"] = grid;
}

void M_Renderer::Deserialize(Json::Value& root)
{
	vSync = root.get("vsync", true).asBool();	
	depthTest = root.get("depthTest", true).asBool();	
	cullFace = root.get("cullFace", true).asBool();	
	wireframe = root.get("wireframe", true).asBool();	
	grid = root.get("grid", true).asBool();
}

void M_Renderer::LoadConfig()
{
	JsonSerializer::DeserializeFormPath(this, App->configPath[name]);
}

void M_Renderer::SaveConfig()
{
	std::string output;
	JsonSerializer::Serialize(this, output, App->configPath[name]);
	SDL_Log("%s", output);
}

void M_Renderer::SetDepthTest(bool stat)
{
	depthTest = stat;
	if (stat)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void M_Renderer::SetCullFace(bool stat)
{
	cullFace = stat;
	if (stat)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void M_Renderer::SetWireframe(bool stat)
{
	wireframe = stat;
}

void M_Renderer::SetGrid(bool stat)
{
	grid = stat;
}

bool M_Renderer::GetDepthTest()
{
	return depthTest;
}

bool M_Renderer::GetCullFace()
{
	return cullFace;
}

bool M_Renderer::GetWireframe()
{
	return wireframe;
}

bool M_Renderer::GetGrid()
{
	return grid;
}

bool M_Renderer::IsVSyncActive()const
{
	return vSync;
}

void M_Renderer::SetVSync(bool set)
{
	vSync = set;
	if (SDL_GL_SetSwapInterval(vSync ? 1 : 0) < 0)
	{
		yogLog("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
}

void M_Renderer::setGPUInfo()
{
	 gpuInfo.brand = (char*)glGetString(GL_RENDERER);

	 uint vendor, deviceId;
	 std::wstring brand;
	 unsigned __int64 videoMemBudget, videoMemUsage, videoMemAvailable, videoMemReserved;

	 if (getGraphicsDeviceInfo(&vendor, &deviceId, &brand, &videoMemBudget, &videoMemUsage, &videoMemAvailable, &videoMemReserved))
	 {
		 gpuInfo.vendor = vendor;
		 gpuInfo.deviceId = deviceId;
		 gpuInfo.videoMemBudget = float(videoMemBudget) / (1024.f * 1024.f);
		 gpuInfo.videoMemUsage = float(videoMemUsage) / (1024.f * 1024.f);
		 gpuInfo.videoMemAvaliable = float(videoMemAvailable) / (1024.f * 1024.f);
		 gpuInfo.videoMemReserved = float(videoMemReserved) / (1024.f * 1024.f);
	 }
}