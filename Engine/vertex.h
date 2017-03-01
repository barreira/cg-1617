#ifndef vertex_h
#define vertex_h

#include <string>


class Vertex {
	class VertexImpl;
	VertexImpl* pimpl;

public:
	Vertex(void);
	Vertex(float x, float y, float z);
	Vertex(const Vertex&);

	float getX(void);
	float getY(void);
	float getZ(void);

	void setX(float x);
	void setY(float y);
	void setZ(float z);

	void add(float x, float y, float z);

	std::string toString();

	~Vertex(void);
};

#endif 

