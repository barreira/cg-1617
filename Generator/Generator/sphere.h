#ifndef sphere_h
#define sphere_h


#include "primitive.h"


class Sphere : public Primitive {
	class SphereImpl;
	SphereImpl* pimpl;

public:
	Sphere(void);
	Sphere(float radius, size_t slices = 1, size_t stacks = 1);

	float getRadius(void);
	size_t getStacks(void);
	size_t getSlices(void);

	void generateVertices(void);

	~Sphere(void);
};

#endif