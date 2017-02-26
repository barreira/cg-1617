#include "plane.h"


class Plane::PlaneImpl {
	float dimX;
	float dimZ;

public:
	PlaneImpl(void)
	{
		dimX = dimZ = 0;
	}


	PlaneImpl(float dimX, float dimZ)
	{
		this->dimX = dimX;
		this->dimZ = dimZ;
	}


	float getDimX(void)
	{
		return dimX;
	}


	float getDimZ(void)
	{
		return dimZ;
	}


	~PlaneImpl(void) = default;
};


Plane::Plane(void) : pimpl{ new PlaneImpl() } {}


Plane::Plane(float dimX, float dimZ)
	: pimpl{ new PlaneImpl(dimX, dimZ) } {}


float Plane::getDimX(void)
{
	return pimpl->getDimX();
}


float Plane::getDimZ(void)
{
	return pimpl->getDimZ();
}


void Plane::generateVertices(void)
{
	float x = pimpl->getDimX() / 2;
	float z = pimpl->getDimZ() / 2;

	addVertex(Vertex(x, 0, -z));
	addVertex(Vertex(-x, 0, -z));
	addVertex(Vertex(-x, 0, z));

	addVertex(Vertex(x, 0, -z));
	addVertex(Vertex(-x, 0, z));
	addVertex(Vertex(x, 0, z));
}


Plane::~Plane(void)
{
	delete pimpl;
	pimpl = NULL;
}







