#ifndef plane_h
#define plane_h


#include <vector>
#include "vertex.h"


class Plane {
	Vertex center;
	float dimX;
	float dimY;
	float dimZ;
	std::vector<Vertex> vertices;

	void generatePlane(void);

public:
	Plane();
	Plane(Vertex origin, float dimX, float dimY, float dimZ);
	float getDimX(void);
	float getDimY(void);
	float getDimZ(void);
	//std::vector<Vertex> getVertices(void);
	std::string toString(void);

};

#endif
