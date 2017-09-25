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
#include "../../../Assimp/Assimp/include/cfileio.h"

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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return ret;
}

bool M_Renderer::Start()
{

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

	basicShader.LoadShader("data/shaders/camera.vertexShader", VERTEX);
	//basicShader.LoadShader("data/shaders/basicVertex.vertexShader", VERTEX);
	basicShader.LoadShader("data/shaders/basicFragment.fragmentShader", FRAGMENT);
	basicShader.CompileProgram(basicShader.vertexShader, basicShader.fragmentShader);

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


	//TMP:
	const aiScene* scene;// = aiImportFile("warrior.FBX", aiProcessPreset_TargetRealtime_MaxQuality);

	char* buffer;
	uint fileSize = App->fs->load("warrior.FBX", &buffer);

	if (buffer && fileSize > 0)
	{
		scene = aiImportFileFromMemory(buffer, fileSize, aiProcessPreset_TargetRealtime_MaxQuality, "fbx");
	}
	else
	{
		yogConsole(CONSOLE_ERROR, "Error while loading fbx.");
		return false;
	}


	if (scene != nullptr && scene->HasMeshes())
	{		
		// use scene->mNumMeshes to iterate on scene->mMeshes array
		//meshes = new VramVertex[scene->mNumMeshes];

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			C_Mesh* mesh = new C_Mesh(nullptr);
			aiMesh* new_mesh = scene->mMeshes[i];
			mesh->Load(new_mesh);
			meshes.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
		yogConsole(CONSOLE_ERROR, "Error loading Scene %s", "warrior.FBX");


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
//	glUseProgram(shaderProgram);
//	glBindVertexArray(VAO);	
//	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(basicShader.shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
							//glDrawArrays(GL_TRIANGLES, 0, 6);
	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "projection"), 1, GL_FALSE, frustum->camera.ProjectionMatrix().Transposed().ptr());
	math::float4x4 view = frustum->camera.ViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(basicShader.shaderProgram, "view"), 1, GL_FALSE, view.Transposed().ptr());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (auto mesh : meshes)
	{
		mesh->Draw(basicShader, frustum);
	}

	glUseProgram(basicShader.shaderProgram);
	//Draw floor grid and world axis
	P_Plane floor(0, 1, 0, 0);
	floor.axis = true;
	floor.color.Set(255, 255, 255);
	floor.Render();
	frustum->Move(dt);
	frustum->Rotate(dt);

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
}

void M_Renderer::Deserialize(Json::Value& root)
{
	vSync = root.get("vsync", true).asBool();	
}

void M_Renderer::LoadConfig()
{
	JsonSerializer::DeserializeFormPath(this, App->configPath[name]);
}

void M_Renderer::SaveConfig()
{
	std::string output;
	JsonSerializer::Serialize(this, output, "config/renderer.json");
	SDL_Log("%s", output);
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