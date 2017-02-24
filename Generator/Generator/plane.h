#ifndef plane_h
#define plane_h

#include "primitive.h"

class Plane : public Primitive {
	class PlaneImpl;
	std::unique_ptr<PlaneImpl> pimpl;

public:
	Plane(void);
	Plane(float dimX, float dimZ);
	
	float getDimX(void);
	float getDimZ(void);
	
	void generateVertices(void);

	~Plane(void);
};

#endif
