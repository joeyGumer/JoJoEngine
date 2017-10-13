#include "ModuleSceneEdit.h"
#include "Primitive.h"
#include "OpenGl.h"
#include "Math.h"

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

ModuleSceneEdit::ModuleSceneEdit(bool start_enabled): Module(start_enabled)
{
	name = "scene editor";
}

ModuleSceneEdit::~ModuleSceneEdit()
{

}

//-----------------------------------

bool ModuleSceneEdit::Start()
{
	wplane = new PWiredPlane(0,1,0,0);
	ex_cube = new PCube(2.0f, 2.0f, 2.0f);
	ex_cylinder = new PCylinder(1.0f, 3.0f);
	ex_sphere = new PSphere(1.0f);
	ex_plane = new PPlane(2.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	world_axis = new PAxis();

	return true;
}

bool ModuleSceneEdit::CleanUp()
{
	delete wplane;
	delete ex_cube;
	delete ex_cylinder;
	delete ex_sphere;
	delete ex_plane;
	delete world_axis;
	return true;
}

//-----------------------------------

update_status ModuleSceneEdit::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEdit::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEdit::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

//------------------------------------

void ModuleSceneEdit::Draw()
{

	
	wplane->Render();
	//ex_cube->Render();
	//ex_cylinder->Render();
	//ex_plane->Render();


	glLineWidth(2.0f);

	//Texture creations with checkers

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	GLuint textureId = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glBindTexture(GL_TEXTURE_2D, 0);

	const uint num_vertices = 8;
	float vertices[8 * 3] =
	{ 
		 1.0f, -1.0f,  1.0f, //0
		 1.0f,  1.0f,  1.0f, //1
		-1.0f,  1.0f,  1.0f, //2
		-1.0f, -1.0f,  1.0f, //3
		 1.0f, -1.0f, -1.0f, //4
		 1.0f,  1.0f, -1.0f, //5
		-1.0f,  1.0f, -1.0f, //6
		-1.0f, -1.0f, -1.0f, //7
	};

	const uint num_uvs = 8;
	float uvs[8 * 2] =
	{

		1.0f, -1.0f, //0
		1.0f, 1.0f, //1
		- 1.0f, 1.0f, //2
		- 1.0f, -1.0f, //3
		- 1.0f, -1.0f, //4
		- 1.0f, 1.0f, //5
		1.0f, 1.0f, //6
		1.0f, -1.0f, //7
	};

	const uint num_indices = 36;
	uint indices[num_indices] =
	{
		0,1,2,
		0,2,3,
		4,5,1,
		4,1,0,
		7,6,5,
		7,5,4,
		3,2,6,
		3,6,7,
		1,5,6,
		1,6,2,
		4,0,3,
		4,3,7
	};

	
	
	uint my_vertices = 0;
	glGenBuffers(1, (GLuint*) &(my_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);

	uint my_uvs = 0;
	glGenBuffers(1, (GLuint*) &(my_uvs));
	glBindBuffer(GL_ARRAY_BUFFER, my_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uvs * 2, uvs, GL_STATIC_DRAW);
	

	//Texture
	glBindTexture(GL_TEXTURE_2D, textureId);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//NOTE: Use atribute pointer?
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, my_uvs);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, my_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);


	//Axis last to be drawn
	world_axis->InnerRender();

}

bool ModuleSceneEdit::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}

bool ModuleSceneEdit::SaveConfig(JSON_Object* data)
{
	bool ret = true;

	return ret;
}