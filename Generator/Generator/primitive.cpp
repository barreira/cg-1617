#include <iterator>
#include "primitive.h"


class Primitive::PrimitiveImpl {
	std::vector<Vertex> vertices;

public:
	PrimitiveImpl(void) = default;


	PrimitiveImpl(std::vector<Vertex> vertices)
	{
		for (size_t i = 0; i < vertices.size(); i++) {
			this->vertices.push_back(vertices.at(i));
		}
	}


	std::vector<Vertex> getVertices(void)
	{
		return vertices;
	}

	
	void addVertex(Vertex v)
	{
		vertices.push_back(v);
	}
	
	
	~PrimitiveImpl(void) = default;
};


Primitive::Primitive(void) : pimpl{ new PrimitiveImpl() } {}


Primitive::Primitive(std::vector<Vertex> vertices)
	: pimpl{ new PrimitiveImpl(vertices) } {}


Primitive::Primitive(const Primitive& p)
	: pimpl{ new PrimitiveImpl(p.pimpl->getVertices()) } {}


std::vector<Vertex> Primitive::getVertices(void)
{
	return pimpl->getVertices();
}


void Primitive::addVertex(Vertex v)
{
	pimpl->addVertex(v);
}


Primitive::~Primitive(void)
{
	delete pimpl;
	pimpl = NULL;
}