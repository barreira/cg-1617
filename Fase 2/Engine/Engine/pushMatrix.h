#ifndef pushMatrix_h
#define pushMatrix_h


#include "glOperation.h"


class PushMatrix : public GLOperation {

public:
	PushMatrix(void) = default;


	void execute(void)
	{
		glPushMatrix();
	}


	~PushMatrix(void) = default;
};


#endif
