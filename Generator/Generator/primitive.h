#ifndef primitive_h
#define primitive_h

#include <vector>
#include "vertex.h"

class Primitive {
	class PrimitiveImpl;
	std::unique_ptr<PrimitiveImpl> pimpl;

public:
	Primitive(void);
	
	void addVertex(Vertex*);
	std::vector<Vertex*> getVertices(void);
	virtual void generateVertices(void) = 0;

	~Primitive(void);
};

#endif 

