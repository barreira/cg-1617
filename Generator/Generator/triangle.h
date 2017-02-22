#ifndef triangle_h
#define triangle_h


#include <vector>
#include "vertex.h"


class Triangle {
	Vertex v1;
	Vertex v2;
	Vertex v3;



public:
	Triangle();
	
	void generateVertices(dimX, dimY, d);
};


#endif
