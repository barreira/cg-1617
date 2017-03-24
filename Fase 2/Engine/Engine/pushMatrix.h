#ifndef pushMatrix_h
#define pushMatrix_h


#include "operation.h"


class PushMatrix : public Operation {

public:
	PushMatrix(void) = default;


	void compute(void)
	{
		glPushMatrix();
	}


	~PushMatrix(void) = default;
};


#endif
