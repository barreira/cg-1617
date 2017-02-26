#ifndef cone_h
#define cone_h

#include "primitive.h"

class Cone : public Primitive {
	class ConeImpl;
	std::unique_ptr<ConeImpl> pimpl;

public:
	Cone(void);
	Cone(float radius, float height, size_t slices = 1, size_t stacks = 1);

	float getRadius(void);
	float getHeight(void);
	double getAlfa(void);
	float getStackHeight(void);
	size_t getStacks(void);
	size_t getSlices(void);

	void generateVertices(void);

	~Cone(void);
};

#endif