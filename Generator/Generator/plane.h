#ifndef plane_h
#define plane_h


#include "primitive.h"


class Plane : public Primitive {
	class PlaneImpl;
	PlaneImpl* pimpl;

public:
	Plane(void);
	Plane(float dimX, float dimZ);
	Plane(const Plane&);
	
	float getDimX(void);
	float getDimZ(void);
	
	void generateVertices(void);

	~Plane(void);
};

#endif
