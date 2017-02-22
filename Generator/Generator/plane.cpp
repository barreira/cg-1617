#include "plane.h"


Plane::Plane(void)
{
	Vertex center;
	
	dimX = dimY = dimZ = 0;
	this->center = center;
	this->generatePlane();
}


Plane::Plane(Vertex center, float dimX, float dimY, float dimZ)
{
	this->dimX = dimX;
	this->dimY = dimY;
	this->dimZ = dimZ;
	this->center = center;
	this->generatePlane();
}


float Plane::getDimX(void)
{
	return dimX;
}


float Plane::getDimY(void)
{
	return dimY;
}



float Plane::getDimZ(void)
{
	return dimZ;
}


std::string Plane::toString(void)
{
	std::string str("");

	for (size_t i = 0; i < vertices.size(); i++) {
		str + vertices.at(i).toString() + "\n";
	}

	return str;
}


void Plane::generatePlane(void)
{
	float x = dimX / 2;
	float y = dimY / 2;
	float z = dimZ / 2;

	vertices.push_back(new Vertex(-x + center.getX(), center.getY, z + center.getZ()));
	
	v1 = generateVertex(x, 0, z, -1, 1, 1);
	v2 = generateVertex(x, 0, z, 1, 1, -1);
}









