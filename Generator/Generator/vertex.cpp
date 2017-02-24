#include <vector>
#include <sstream>
#include "vertex.h"


class Vertex::VertexImpl {
	float x;
	float y;
	float z;

public :
	VertexImpl(void) 
	{
		x = y = z = 0;
	}


	VertexImpl(float x, float y, float z) 
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	float getX(void) 
	{
		return x;
	}


	float getY(void)
	{
		return y;
	}


	float getZ(void)
	{
		return z;
	}


	void setX(float x)
	{
		this->x = x;
	}


	void setY(float y)
	{
		this->y = y;
	}


	void setZ(float z)
	{
		this->z = z;
	}


	void add(float x, float y, float z)
	{
		this->x += x;
		this->y += y;
		this->z += z;
	}


	std::string toString(void)
	{
		std::stringstream aux;

		aux << x << "," << y << "," << z;

		return aux.str();
	}


	~VertexImpl(void) = default;
};



Vertex::Vertex(void) : pimpl{ new VertexImpl() } {}


Vertex::Vertex(float x, float y, float z) 
	: pimpl{ new VertexImpl(x, y, z) } {}


/*
Vertex::Vertex(const Vertex& v) : pimpl{ new VertexImpl() } 
{
	pimpl->setX(v.pimpl->getX());
	pimpl->setX(v.pimpl->getY());
	pimpl->setX(v.pimpl->getZ());
}*/


float Vertex::getX(void)
{
	return pimpl->getX();
}


float Vertex::getY(void)
{
	return pimpl->getY();
}


float Vertex::getZ(void)
{
	return pimpl->getZ();
}


void Vertex::setX(float x)
{
	pimpl->setX(x);
}


void Vertex::setY(float y)
{
	pimpl->setY(y);
}


void Vertex::setZ(float z)
{
	pimpl->setZ(z);
}


void Vertex::add(float x, float y, float z)
{
	pimpl->add(x, y, z);
}


std::string Vertex::toString(void)
{
	return pimpl->toString();
}


Vertex::~Vertex(void) = default;
