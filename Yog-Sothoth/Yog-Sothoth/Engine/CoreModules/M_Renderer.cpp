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
#pragma region plane
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		3, 1, 0,  // first Triangle
		3, 2, 1   // second Triangle
	};

#pragma endregion
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
#pragma region Cube
	float vertices_cube[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
#pragma endregion
	lightPos = { 1.2f, 1.0f, 2.0f };

	basicShader.LoadShader("data/shaders/camera.vs", VERTEX);
	basicShader.LoadShader("data/shaders/basicFragment.fs", FRAGMENT);
	basicShader.CompileProgram(basicShader.vertexShader, basicShader.fragmentShader);
	lightShader.LoadShader("data/shaders/1.colors.vs", VERTEX);
	lightShader.LoadShader("data/shaders/1.colors.fs", FRAGMENT);
	lightShader.CompileProgram(lightShader.vertexShader, lightShader.fragmentShader);
	lampShader.LoadShader("data/shaders/1.lamp.vs", VERTEX);
	lampShader.LoadShader("data/shaders/1.lamp.fs", FRAGMENT);
	lampShader.CompileProgram(lampShader.vertexShader, lampShader.fragmentShader);
#pragma region planeGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


#pragma endregion

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//###########################################

	//App->objManager->LoadFBX("warrior.FBX");
	//App->objManager->LoadFBX("Street environment_V01.FBX");
	//App->objManager->LoadFBX("MechaT.FBX");

	return true;
}

update_status M_Renderer::PreUpdate(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

//	glUseProgram(basicShader.shaderProgram);
//	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//							//glDrawArrays(GL_TRIANGLES, 0, 6);
//	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "projection"), 1, GL_FALSE, frustum->camera.ProjectionMatrix().Transposed().ptr());
	math::float4x4 view = frustum->camera.ViewMatrix();
//	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "view"), 1, GL_FALSE, view.Transposed().ptr());
//	glUniform4f(glGetUniformLocation(basicShader.shaderProgram, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//	glUseProgram(0);

	glUseProgram(lightShader.shaderProgram);
	glUniform3f(glGetUniformLocation(lightShader.shaderProgram, "objectColor"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(lightShader.shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, frustum->camera.ProjectionMatrix().Transposed().ptr());
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, view.Transposed().ptr());

	float3 position(0, 0, 0);
	float3 scale(1, 1, 1);
	Quat rotation = Quat::identity;
	float4x4 model(rotation, position);
	model.Scale(scale);

	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, model.Transposed().ptr());
	// render the cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// also draw the lamp object
	glUseProgram(lampShader.shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(lampShader.shaderProgram, "projection"), 1, GL_FALSE, frustum->camera.ProjectionMatrix().Transposed().ptr());
	glUniformMatrix4fv(glGetUniformLocation(lampShader.shaderProgram, "view"), 1, GL_FALSE, view.Transposed().ptr());
	
	float3 pos;
	float3 sca;
	Quat rot;
	model.Decompose(pos, rot, sca);
	sca *= 0.2f;
	model = float4x4::FromTRS(lightPos, rot, sca);

	//model.Scale(0.2f, 0.2f, 0.2f); // a smaller cube
	glUniformMatrix4fv(glGetUniformLocation(lampShader.shaderProgram, "model"), 1, GL_FALSE, model.Transposed().ptr());

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glUseProgram(0);

	App->objManager->root->Draw(basicShader, frustum);
	

	//Draw floor grid and world axis
	if (grid)
	{
		glPushMatrix();
		glMultMatrixf(frustum->camera.ProjectionMatrix().Transposed().ptr());
		glPushMatrix();
		glMultMatrixf(view.Transposed().ptr());
		P_Plane floor(0, 1, 0, 0);
		floor.color = { 0.9f, 0.9f, 0.9f , 1.f };
		floor.axis = true;
		floor.Render();
		glPopMatrix();
		glPopMatrix();

	}

	frustum->Move(dt);
	frustum->Rotate(dt);

//	glPushMatrix();
//	glMultMatrixf(frustum->camera.ProjectionMatrix().Transposed().ptr());
//	glPushMatrix();
//	glMultMatrixf(view.Transposed().ptr());

//	// Draw a cube with 12 triangles
//	glBegin(GL_TRIANGLES);
//
//	// front face
//	glColor3d(255, 255, 0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(10.0f, 0.0f, 0.0f);
//	glVertex3f(10.0f, 10.0f, 0.0f);
//
//	glVertex3f(10.0f, 10.0f, 0.0f);
//	glVertex3f(0.0f, 10.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//
//	// left face
//	glColor3d(255, 0, 0);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, 10.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, -10.0f);
//
//	glVertex3f(0.0f, 0.0f, -10.0f);
//	glVertex3f(0.0f, 10.0f, 0.0f);
//	glVertex3f(0.0f, 10.0f, -10.0f);
//
//	// top face 
//	glColor3d(0, 255, 0);
//	glVertex3f(0.0f, 10.0f, 0.0f);
//	glVertex3f(10.0f, 10.0f, 0.0f);
//	glVertex3f(10.0f, 10.0f, -10.0f);
//
//	glVertex3f(0.0f, 10.0f, 0.0f);
//	glVertex3f(10.0f, 10.0f, -10.0f);
//	glVertex3f(0.0f, 10.0f, -10.0f);
//
//	// right face
//	glColor3d(0, 0, 255);
//
//	glVertex3f(10.0f, 0.0f, 0.0f);
//	glVertex3f(10.0f, 0.0f, -10.0f);
//	glVertex3f(10.0f, 10.0f, 0.0f);
//
//	glVertex3f(10.0f, 10.0f, 0.0f);
//	glVertex3f(10.0f, 0.0f, -10.0f);
//	glVertex3f(10.0f, 10.0f, -10.0f);
//
//	// back face
//	glColor3d(0, 255, 255);
//
//	glVertex3f(0.0f, 0.0f, -10.0f);
//	glVertex3f(10.0f, 10.0f, -10.0f);
//	glVertex3f(10.0f, 0.0f, -10.0f);
//
//	glVertex3f(0.0f, 10.0f, -10.0f);
//	glVertex3f(10.0f, 10.0f, -10.0f);
//	glVertex3f(0.0f, 0.0f, -10.0f);
//
//	// top face
//	glColor3d(255, 0, 255);
//
//	glVertex3f(10.0f, 0.0f, -10.0f);
//	glVertex3f(10.0f, 0.0f, 0.0f);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//
//	glVertex3f(0.0f, 0.0f, -10.0f);
//	glVertex3f(10.0f, 0.0f, -10.0f);
//	glVertex3f(0.0f, 0.0f, 0.0f);
//
//	glEnd();
//	glPopMatrix();
//	glPopMatrix();

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