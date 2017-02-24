#include "primitive.h"


class Primitive::PrimitiveImpl {
	std::vector<Vertex*> vertices;

public:
	PrimitiveImpl(void) = default;


	void addVertex(Vertex* v)
	{
		vertices.push_back(v);
	}


	std::vector<Vertex*> getVertices(void)
	{
		return vertices;
	}


	~PrimitiveImpl(void) = default;
};


Primitive::Primitive(void) : pimpl{ new PrimitiveImpl() } {}


void Primitive::addVertex(Vertex* v)
{
	pimpl->addVertex(v);
}


std::vector<Vertex*> Primitive::getVertices(void)
{
	return pimpl->getVertices();
}


Primitive::~Primitive(void) = default;