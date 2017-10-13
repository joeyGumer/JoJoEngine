
#include "Globals.h"
#include "Primitive.h"
#include "OpenGl.h"
#include <vector>

//TODO : change all primitives inner render to VBO mode and separating geometry creation

// ------------------------------------------------------------
Primitive::Primitive() : transform(transform.identity), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.v);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	InnerRender();

	glPopMatrix();

}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

//-------------------------------------------------------------
void Primitive::CreateGeometry()
{

}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.Translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const float3 &u)
{
	transform.RotateAxisAngle(u, angle);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.Scale(x, y, z);
}

// ------------------------------------------------------------
float3 Primitive::GetPos()const
{
	//NOTE: If error getting position, check matrix given coordinates
	float3 ret;
	ret.x = transform[0][3];
	ret.y = transform[1][3];
	ret.z = transform[2][3];

	return ret;
}

// CUBE ============================================
PCube::PCube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;

	CreateGeometry();
}

PCube::PCube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;

	CreateGeometry();
}

void PCube::CreateGeometry()
{

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	//Cube vertex array
	vertices =
	{
		sx, -sy, sz,
		sx, sy, sz,
		-sx, sy, sz,
		-sz, -sy, sz,
		sx, -sy, -sz,
		sx, sy, -sz,
		-sx, sy, -sz,
		-sx, -sy, -sz
	};

	//Cube index array
	indices =
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

}

void PCube::InnerRender() const
{	
	glGenBuffers(1, (GLuint*) &(my_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* indices.size(), &indices[0], GL_STATIC_DRAW);

	//Draw 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, my_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	/*
	//Cube vertex array
	const uint num_vertices = 8;
	float vertices[8 * 3] =
	{
		 sx, -sy,  sz,
		 sx,  sy,  sz,
		-sx,  sy,  sz,
		-sz, -sy,  sz,
		 sx, -sy, -sz,
		 sx,  sy, -sz,
		-sx,  sy, -sz,
		-sx, -sy, -sz,
	};
	uint my_vertices = 0;
	glGenBuffers(1, (GLuint*) &(my_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);

	//Cube index array
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

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);
	

	//Draw 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, my_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);*/

	
}

// SPHERE ============================================
PSphere::PSphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

PSphere::PSphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void PSphere::InnerRender() const
{
	//glutSolidSphere(radius, 25, 25);
}


// CYLINDER ============================================
PCylinder::PCylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

PCylinder::PCylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void PCylinder::InnerRender() const
{
	int n = 30;

	//Vertex array-----------------------------------

	uint num_vertices = (n * 2) + 2;

	//NOTE: still not dynamically supported
	float vertices[(30 * 2 * 3) + 6];

	uint vertex_id = 0;

	//Top--------------------------

	//Top tip
	vertices[vertex_id++] = 0.0f;
	vertices[vertex_id++] = height * 0.5;
	vertices[vertex_id++] = 0.0f;

	for (uint f = 0; f < 360; f += (360 / n), vertex_id += 3)
	{
		float a = float(f) * M_PI / 180; // degrees to radians
		vertices[vertex_id] = radius * cos(a);
		vertices[vertex_id + 1] = height * 0.5;
		vertices[vertex_id + 2] = radius * sin(a);
	}

	//Bottom----------------------

	//Bottom tip
	vertices[vertex_id++] = 0.0f;
	vertices[vertex_id++] = -height * 0.5;
	vertices[vertex_id++] = 0.0f;

	for (uint f = 0; f < 360; f += (360 / n), vertex_id += 3)
	{
		float a = float(f) * M_PI / 180; // degrees to radians
		vertices[vertex_id] = radius * cos(a);
		vertices[vertex_id + 1] = -height * 0.5;
		vertices[vertex_id + 2] = radius * sin(a);
	}

	uint my_vertices = 0;
	glGenBuffers(1, (GLuint*) &(my_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);

	//Indices array-----------------------------------------------
	const uint num_indices = n * 12;
	uint indices[30 * 12]; //NOTE: doesn't accept variables

	uint index_id = 0;
	//Top


	uint start = 1, end = n;
	for (int i = start; i < n; i++, index_id+=3)
	{
		indices[index_id] = i;
		indices[index_id + 1] = 0;
		indices[index_id + 2] = i + 1;
	}
	indices[index_id++] = end;
	indices[index_id++] = 0;
	indices[index_id++] = start;
	
	//Bottom
	start = n + 2, end = n * 2;
	for (int i = end; i > start; i--, index_id += 3)
	{
		//NOTE: If bottom doesn't paint, here's the cause
		indices[index_id] = i;
		indices[index_id + 1] = n + 1;
		indices[index_id + 2] = i - 1;
	}
	indices[index_id++] = start;
	indices[index_id++] = n + 1;
	indices[index_id++] = end;

	//Sides
	for (int i = 1; i < n; i++, index_id += 6)
	{
		indices[index_id] = i;
		indices[index_id + 1] = i + 1;
		indices[index_id + 2] = i + n +1;

		indices[index_id + 3] = i;
		indices[index_id + 4] = i + n + 1;
		indices[index_id + 5] = i + n;
	}

	indices[index_id++] = n;
	indices[index_id++] = n + 2;
	indices[index_id++] = 1;

	indices[index_id++] = n;
	indices[index_id++] = n + n;
	indices[index_id++] = n + 2;

	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, indices, GL_STATIC_DRAW);

	//Draw 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, my_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);


	// Cylinder Bottom
	/*glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();*/
}

// LINE ==================================================
PLine::PLine() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

PLine::PLine(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}



void PLine::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
PPlane::PPlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;

	CreateGeometry();
}

PPlane::PPlane(float s, float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d), size(s)
{
	size = s;
	type = PrimitiveTypes::Primitive_Plane;

	CreateGeometry();
}

void PPlane::CreateGeometry()
{
	//Renders double face NOTE: look if there's another way to do it with OpenGL
	float s = size * 0.5f;

	vertices =
	{
		s, 0, s,
		s, 0 ,-s,
		-s, 0, -s,
		-s, 0, s
	};

	indices =
	{
		0,1,2,
		0,2,3,
		0,2,1,
		0,3,2
	};
}

void PPlane::InnerRender() const
{

	glGenBuffers(1, (GLuint*) &(my_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* indices.size(), &indices[0], GL_STATIC_DRAW);

	//Draw 	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, my_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//WIRED PLANE ==================================================
PWiredPlane::PWiredPlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

PWiredPlane::PWiredPlane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void PWiredPlane::InnerRender() const
{
	glLineWidth(1.0f);
	float d = 200.0f;

	//NOTE: wired plane to confusion to do with indices
	/*uint num_vertices = 400;

	float vertices[400 * 3];
	memset(vertices, 0, 400 * 3);

	
	for (int i = 0, float f = -d; f <= d; f += 1.0f, i+=12)
	{
		vertices[i] = f;
		vertices[i + 2] = -d;
		vertices[i + 3] = f;
		vertices[i + 5] = d;
		vertices[i + 6] = -d;
		vertices[i + 8] = f;
		vertices[i + 9] = d;
		vertices[i + 11] = f;
	}*/

	glBegin(GL_LINES);

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

PAxis::PAxis() : Primitive()
{

}

void PAxis::InnerRender() const
{
	glLineWidth(5.0f);

	glBegin(GL_LINES);

	float size = 3.0f;

	glColor3f(0.0, 1.0, 0.0);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(size, 0.0, 0.0);

	glColor3f(1.0, 0.0, 0.0);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, size, 0.0);

	glColor3f(0.0, 0.0, 1.0);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, size);

	glEnd();

}