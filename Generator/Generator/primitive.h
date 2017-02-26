#ifndef primitive_h
#define primitive_h

#include <vector>
#include "vertex.h"

class Primitive {
	class PrimitiveImpl;
	PrimitiveImpl* pimpl;

public:
	Primitive(void);
	Primitive(std::vector<Vertex>);
	Primitive(const Primitive&);
	
	std::vector<Vertex> getVertices(void);
	void addVertex(Vertex);
	virtual void generateVertices(void) = 0;

	virtual ~Primitive(void);
};

#endif 

