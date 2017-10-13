#include "Globals.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneEdit.h"
#include "ModuleEditor.h"
#include "ModuleFBXLoader.h"

#include "JSON\parson.h"
#include "OpenGl.h"

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"

//Devil NOTE: may not be better to put it here
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment(lib, "Devil/libx86/DevIL.lib")
#pragma comment(lib, "Devil/libx86/ILU.lib")
#pragma comment(lib, "Devil/libx86/ILUT.lib")


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module( start_enabled)
{
	name = "renderer3D";

	num_meshes = 0;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Initialize Devil NOTE: may have to be ported
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	//Create OpenGL context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum gl_enum = glewInit();

	if (GLEW_OK != gl_enum)
	{
		LOG("Glew failed");
	}
	else
	{
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Detecting Hardware and driver compatibilities
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGl version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		//OpenGl Initialization 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);


		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT, 20.0f);

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

//NOTE: look how to organize the order of loading
bool ModuleRenderer3D::Start()
{
	bool ret = true;

	//LoadImageTexture("Baker_house.png");
	//LoadMesh("BakerHouse.FBX");

	//NOTE: For now, use this texture only

	return true;
}
// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	ImGui_ImplSdlGL3_Shutdown();

	//Delete all meshes
	uint size = meshes_array.capacity();
	for (uint i = 0; i < size; i++)
	{
		RELEASE(meshes_array[i]);
	}
	
	meshes_array.clear();

	//Delete OpenGl context
	SDL_GL_DeleteContext(context);

	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	//Color c = App->camera->background;
	//glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Draw 
	App->level->Draw();
	DrawMeshes();
	/*if (debug_draw == true)
	{
		BeginDebugDraw();
		App->DebugDraw();
		EndDebugDraw();
	}*/
	App->editor->Draw();
	
	//Swap window for next frame
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

bool ModuleRenderer3D::LoadMesh(char* file)
{
	bool ret = true;

	//NOTE: temporal, have to configure library and assets directory
	Mesh** meshes = App->fbx->LoadFBX(file, &num_meshes);

	if (meshes != nullptr)
	{
		for (uint i = 0; i < num_meshes; i++)
		{
			meshes_array.push_back(meshes[i]);

			//NOTE: Still not sure if this is the best spot to generate the buffers
			glGenBuffers(1, (GLuint*) &(meshes[i]->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)* meshes[i]->num_vertices * 3, meshes[i]->vertices, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*) &(meshes[i]->id_texture_UVs));
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->id_texture_UVs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)* meshes[i]->num_texture_UVs * 2, meshes[i]->texture_UVs, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*) &(meshes[i]->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* meshes[i]->num_indices, meshes[i]->indices, GL_STATIC_DRAW);

		}
	}
	else
	{
		ret = false;
		LOG("Error: no meshes found to load");
	}

	return ret;
}

bool ModuleRenderer3D::LoadImageTexture(char* file)
{
	bool ret = true;
	ILuint id_image;
	ilGenImages(1, &id_image);
	ilBindImage(id_image);

	ret = ilLoadImage(file);

	if (ret)
	{
		//NOTE: maybe check if the image is a power of two?

		texture_channel = ilutGLBindTexImage();


		/*texture_channel = 0;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texture_channel);
		glBindTexture(GL_TEXTURE_2D, texture_channel);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 
			0, 
			ilGetInteger(IL_IMAGE_FORMAT), 
			ilGetInteger(IL_IMAGE_WIDTH),   // Image width
			ilGetInteger(IL_IMAGE_HEIGHT), 
			0, 
			ilGetInteger(IL_IMAGE_FORMAT), //NOTE: may have to change this
			GL_UNSIGNED_BYTE, 
			ilGetData());

		glBindTexture(GL_TEXTURE_2D, 0);*/
	}
	else
	{
		LOG("Error: failure trying to load texture %s", file);
	}

	ilDeleteImages(1, &id_image);

	return ret;
}
//NOTE: pass as references? or pointer? or copy?
void ModuleRenderer3D::Draw(const Mesh* mesh) const
{
	glColor3f(1.0f, 1.0f, 1.0f);

	//Draw 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_UVs);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, texture_channel);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	//Wireframe
	DrawWireframe(mesh);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	glBindTexture(GL_TEXTURE_2D, 0);

}

void ModuleRenderer3D::DrawNormals(const Mesh* mesh) const
{
	if (draw_normals)
	{
		//NOTE: trying to draw them with direct mode, temporaly
		glBegin(GL_LINES);

		glLineWidth(1.0f);
		glColor3f(0.0f, 1.0, 1.0);

		for (int i = 0; i < mesh->num_normals * 3; i += 3)
		{
			float3 vertex = { mesh->vertices[i], mesh->vertices[i + 1], mesh->vertices[i + 2] };
			float3 n_vertex = vertex + float3(mesh->normals[i], mesh->normals[i + 1], mesh->normals[i + 2]);

			glVertex3f(vertex.x, vertex.y, vertex.z);
			glVertex3f(n_vertex.x, n_vertex.y, n_vertex.z);
		}

		glEnd();
	}
}

void ModuleRenderer3D::DrawWireframe(const Mesh* mesh) const
{
	if (draw_wireframe)
	{
		

		glColor3f(1.0f, 1.0f, 0.0f);
		glLineWidth(2.0f);

		if(texture_enabled)
			glDisable(GL_TEXTURE_2D);

		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);

		if (texture_enabled)
			glEnable(GL_TEXTURE_2D);

		glEnable(GL_LIGHTING);
	}
}


void ModuleRenderer3D::DrawMeshes() const
{

	for (uint i = 0; i < num_meshes; i++)
	{
		Draw(meshes_array[i]);
		DrawNormals(meshes_array[i]);
	}
}

void ModuleRenderer3D::OnResize(int width, int height, float fovy)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(fovy, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Utiles

void ModuleRenderer3D::EnableTextures(bool enable)
{
	texture_enabled = enable;

	if (texture_enabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

}

bool ModuleRenderer3D::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}

bool ModuleRenderer3D::SaveConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}