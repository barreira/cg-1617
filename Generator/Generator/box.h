#ifndef box_h
#define box_h


#include "primitive.h"


class Box : public Primitive {
	class BoxImpl;
	BoxImpl* pimpl;

public:
	Box(void);
	Box(float dimX, float dimY, float dimZ, size_t numDiv = 0);
	Box(const Box&);

	float getDimX(void);
	float getDimY(void);
	float getDimZ(void);
	size_t getNumDiv(void);

	void generateVertices(void);

	~Box(void);
};

#endif
