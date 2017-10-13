
#include "Math.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;
	float3		GetPos()const;

protected: 
	virtual void	CreateGeometry();

public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;

	std::vector<float> vertices;
	std::vector<uint> indices;
	uint my_vertices, my_indices = 0;

protected:
	PrimitiveTypes type;

};

// ============================================
class PCube : public Primitive
{
public :
	PCube();
	PCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;

private :
	void	CreateGeometry();

public:
	float3 size;
};

// ============================================
class PSphere : public Primitive
{
public:
	PSphere();
	PSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class PCylinder : public Primitive
{
public:
	PCylinder();
	PCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PLine : public Primitive
{
public:
	PLine();
	PLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class PPlane : public Primitive
{
public:
	PPlane();
	PPlane(float size, float x, float y, float z, float d);
	void InnerRender() const;

private:
	void CreateGeometry();

public:
	float3 normal;
	float size;
	float constant;
};

// ============================================
class PWiredPlane : public Primitive
{
public:
	PWiredPlane();
	PWiredPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};

//=============================================
class PAxis : public Primitive
{
public: 
	PAxis();
	void InnerRender() const;
};