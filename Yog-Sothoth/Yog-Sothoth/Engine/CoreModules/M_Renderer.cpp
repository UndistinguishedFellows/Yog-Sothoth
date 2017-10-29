#include "../../Globals.h"
#include "../../Application.h"
#include "M_Renderer.h"
#include "M_Window.h"
#include "../../OpenGL.h"
#include "../../Tools/Static/JsonSerializer.h"
#include "../UI_Modules/M_UIManager.h"
#include "../../../Assimp/Assimp/include/scene.h"
#include "../GameObjects/Components/C_Transform.h"
#include "../GameObjects/Components/C_Camera.h"
#include "../GameObjects/Components/C_Mesh.h"
#include "../../MathGeoLib/MathGeoLib.h"
#include "../GameObjects/M_ObjectManager.h"
#include "../GameObjects/GameObject.h"
#include "../../Tools/Containers/Shader.h"
#include "../../Tools/Primitive.h"
#include "../../Tools/GPUDetect/DeviceId.h"
#include "../../Tools/Primitives.h"

M_Renderer::M_Renderer(bool enabled) : Module(enabled)
{
	name.assign("renderer");
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
		//glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);
		GLint depthBytes;
		glGetIntegerv(GL_DEPTH_BITS, &depthBytes);
		yogConsole(CONSOLE_INFO, "Depth Bytes: %d", depthBytes);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

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
	wireframeShader = new Shader("data/shaders/wireframe.vs", "data/shaders/wireframe.fs");
	normalShader = new Shader("data/shaders/normalDisplay.vs", 
							  "data/shaders/normalDisplay.fs", 
							  "data/shaders/normalDisplay.gs");
	
	createCheckersTexture();
	

	return true;
}

update_status M_Renderer::PreUpdate(float dt)
{
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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

	float4x4 view = App->objManager->activeCamera->Camera->frustum.ViewMatrix();

	int textureBuffer = 0;
	int prevTextureBuffer = 0;

	drawVector = App->objManager->activeCamera->Camera->GetElementsToDraw();

	for (auto game_object : drawVector)
	{
		if (game_object->Mesh != nullptr)
		{
			if (game_object->Mesh->wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			float4x4 view = App->objManager->activeCamera->Camera->frustum.ViewMatrix();
			//Shader* activeshader = game_object->shader;
			Shader* activeshader = lightShader;

			activeshader->Use();
			activeshader->setInt("tex", 0);
			if (game_object->Mesh->associatedMaterial != nullptr)
			{
				if (game_object->Mesh->associatedMaterial->texture != 0 || game_object->Mesh->associatedMaterial->checkers)
				{
					activeshader->setInt("hasTex", 1);
				}
				else
					activeshader->setInt("hasTex", 0);
			}
			else
			{
				activeshader->setInt("hasTex", 0);
			}

			glBindVertexArray(game_object->Mesh->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
													   //glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, game_object->Mesh->indices.idIndices);

			activeshader->setMat4("projection", &App->objManager->activeCamera->Camera->frustum.ProjectionMatrix().Transposed());
			activeshader->setMat4("view", &view.Transposed());
			if (App->renderer->fbxViewer)
			{
				activeshader->setMat4("model", &float4x4::identity);
			}
			else
			{
				activeshader->setMat4("model", &game_object->Transform->globalTransform.Transposed());
			}
			Color color = game_object->Mesh->color;
			activeshader->setVec3("objectColor", color.r, color.g, color.b);
			activeshader->setVec3("lightColor", 0.50f, 0.50f, 0.50f);
			activeshader->setVec3("lightPos", &App->objManager->testLight->Transform->GetPosition());
			activeshader->setVec3("viewPos", &App->objManager->activeCamera->Camera->frustum.pos);

			glActiveTexture(GL_TEXTURE0);
			if (game_object->Mesh->associatedMaterial->checkers)
			{
				textureBuffer = App->renderer->checkTexture;
			}
			else
			{
				textureBuffer = game_object->Mesh->associatedMaterial->texture;
			}

			if (textureBuffer != prevTextureBuffer)
			{
				glBindTexture(GL_TEXTURE_2D, textureBuffer);
				prevTextureBuffer = textureBuffer;
			}

			glDrawElements(GL_TRIANGLES, game_object->Mesh->indices.numIndices, GL_UNSIGNED_INT, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			if (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//draw Wireframe when selected
	if (App->objManager->focus != nullptr && App->objManager->focus->Mesh != nullptr)
	{
		Color color = App->objManager->focus->Mesh->color;
		App->objManager->focus->Mesh->color.Set(0.f, 1.f, 0.f, 1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glLineWidth(1.f);
		App->objManager->Draw(App->objManager->focus, *wireframeShader);
		//glLineWidth(1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		App->objManager->focus->Mesh->color = color;
	}

	//Draw normals of a mesh
	App->objManager->DrawNormals(App->objManager->root, *normalShader);
	bool cull = cullFace;
	SetCullFace(false);
	App->objManager->DrawAABB(App->objManager->dragAndDropVisualizer, *basicShader);
	SetCullFace(cull);

	//Draw floor grid and world axis
	if (grid)
	{
		glPushMatrix();
		glMultMatrixf(App->objManager->activeCamera->Camera->frustum.ProjectionMatrix().Transposed().ptr());
		glPushMatrix();
		glMultMatrixf(view.Transposed().ptr());
		P_Plane floor(0, 1, 0, 0);
		floor.color = { 0.1f, 0.1f, 0.1f , 1.f };
		floor.axis = true;
		floor.Render();

		glPopMatrix();
		glPopMatrix();

	}






//	if (checkersCube)
//	{
//		glPushMatrix();
//		glMultMatrixf(frustum->camera.ProjectionMatrix().Transposed().ptr());
//		glPushMatrix();
//		glMultMatrixf(view.Transposed().ptr());
//		drawCubeDirectModeTexCoord();
//		glPopMatrix();
//		glPopMatrix();
//	}
//
//	glPushMatrix();
//	glMultMatrixf(frustum->camera.ProjectionMatrix().Transposed().ptr());
//	glPushMatrix();
//	glMultMatrixf(view.Transposed().ptr());
//	drawCubeDirectModeTexture();
//	glPopMatrix();
//	glPopMatrix();

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
	root["fbxViewer"] = fbxViewer;
}

void M_Renderer::Deserialize(Json::Value& root)
{
	vSync = root.get("vsync", true).asBool();	
	depthTest = root.get("depthTest", true).asBool();	
	cullFace = root.get("cullFace", true).asBool();	
	wireframe = root.get("wireframe", true).asBool();	
	grid = root.get("grid", true).asBool();
	fbxViewer = root.get("fbxViewer", true).asBool();
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

bool M_Renderer::IsFBXViewerActive() const
{
	return fbxViewer;
}

void M_Renderer::setFBXViewer(bool set)
{
	fbxViewer = set;
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

void M_Renderer::createCheckersTexture()
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkTexture);
	glBindTexture(GL_TEXTURE_2D, checkTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void M_Renderer::drawCubeDirectModeTexCoord()
{
	glBindTexture(GL_TEXTURE_2D, checkTexture);

	// Draw a cube with 12 triangles
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	// front face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	
	// left face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, -10.0f);

	// top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, -10.0f);

	// right face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);

	// back face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 10.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);

	// top face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void M_Renderer::drawCubeDirectModeTexture()
{
	
	// Draw a cube with 12 triangles
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureLenna);
	glBegin(GL_TRIANGLES);

	// front face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);

	// left face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, -10.0f);

	// top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 10.0f, -10.0f);

	// right face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);

	// back face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 10.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 10.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);

	// top face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}