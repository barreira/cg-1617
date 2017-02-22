#include <vector>
#include <sstream>
#include "vertex.h"


/*
class VertexImp : public Vertex {
	float x;
	float y;
	float z;

public :
	VertexImp(void) 
	{
		x = y = z = 0;
	}


	VertexImp(float x, float y, float z) 
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	float getX() 
	{
		return x;
	}


	float getY()
	{
		return y;
	}


	float getZ()
	{
		return z;
	}


	std::string toString()
	{
		std::stringstream aux;

		aux << x << "," << y << "," << z;
		
		return aux.str();
	}
};
*/



Vertex::Vertex(void)
{
	x = y = z = 0;
}


Vertex::Vertex(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


float Vertex::getX()
{
	return x;
}


float Vertex::getY()
{
	return y;
}


float Vertex::getZ()
{
	return z;
}


std::string Vertex::toString()
{
	std::stringstream aux;

	aux << x << "," << y << "," << z;

	return aux.str();
}

