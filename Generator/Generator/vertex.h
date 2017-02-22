#ifndef vertex_h
#define vertex_h

#include <string>

class Vertex {
	float x;
	float y;
	float z;

public:
	Vertex();
	Vertex(float x, float y, float z);

	float getX(void);
	float getY(void);
	float getZ(void);
	std::string toString();
};

#endif 

