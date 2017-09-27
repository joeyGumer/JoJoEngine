#include "ModuleSceneEdit.h"
#include "OpenGl.h"
#include "Math.h"

ModuleSceneEdit::ModuleSceneEdit(bool start_enabled): Module(start_enabled)
{

}

ModuleSceneEdit::~ModuleSceneEdit()
{

}

//-----------------------------------

bool ModuleSceneEdit::Start()
{
	return true;
}

bool ModuleSceneEdit::CleanUp()
{
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
	glLineWidth(2.0f);

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

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, my_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	//Front
		/*glVertex3f( 1.0f,  -1.0f,   1.0f);	//A
		glVertex3f( 1.0f,	1.0f,	1.0f);	//B
		glVertex3f(-1.0f,   1.0f,	1.0f);	//C
		glVertex3f( 1.0f, -1.0f, 1.0f);		//A
		glVertex3f(-1.0f, 1.0f, 1.0f);		//C
		glVertex3f(-1.0f, -1.0f, 1.0f);		//D
		//Right
		glVertex3f(1.0f, -1.0f, -1.0f);		//E
		glVertex3f(1.0f, 1.0f, -1.0f);		//F
		glVertex3f(1.0f, 1.0f, 1.0f);		//B
		glVertex3f(1.0f, -1.0f, -1.0f);		//E
		glVertex3f(1.0f, 1.0f, 1.0f);		//B
		glVertex3f(1.0f, -1.0f, 1.0f);		//A
		//Back
		glVertex3f(-1.0f, -1.0f, -1.0f);	//H
		glVertex3f(-1.0f, 1.0f, -1.0f);		//G
		glVertex3f(1.0f, 1.0f, -1.0f);		//F
		glVertex3f(-1.0f, -1.0f, -1.0f);	//H
		glVertex3f(1.0f, 1.0f, -1.0f);		//F
		glVertex3f(1.0f, -1.0f, -1.0f);		//E
		//Left
		glVertex3f(-1.0f, -1.0f, 1.0f);		//D
		glVertex3f(-1.0f, 1.0f, 1.0f);		//C
		glVertex3f(-1.0f, 1.0f, -1.0f);		//G
		glVertex3f(-1.0f, -1.0f, 1.0f);		//D
		glVertex3f(-1.0f, 1.0f, -1.0f);		//G
		glVertex3f(-1.0f, -1.0f, -1.0f);	//H
		//TOP
		glVertex3f(1.0f, 1.0f, 1.0f);		//B
		glVertex3f(1.0f, 1.0f, -1.0f);		//F
		glVertex3f(-1.0f, 1.0f, -1.0f);		//G
		glVertex3f(1.0f, 1.0f, 1.0f);		//B
		glVertex3f(-1.0f, 1.0f, -1.0f);		//G
		glVertex3f(-1.0f, 1.0f, 1.0f);		//C
		//BOTTOM
		glVertex3f(1.0f, -1.0f, -1.0f);		//E
		glVertex3f(1.0f, -1.0f, 1.0f);		//A
		glVertex3f(-1.0f, -1.0f, 1.0f);		//D
		glVertex3f(1.0f, -1.0f, -1.0f);		//E
		glVertex3f(-1.0f, -1.0f, 1.0f);		//D
		glVertex3f(-1.0f, -1.0f, -1.0f);	//H*/
	

	glLineWidth(1.0f);
}