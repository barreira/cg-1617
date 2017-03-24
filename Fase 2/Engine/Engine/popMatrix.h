#ifndef popMatrix_h
#define popMatrix_h


#include "operation.h"


class PopMatrix : public Operation {

public:
	PopMatrix(void) = default;


	void compute(void)
	{
		glPopMatrix();
	}


	~PopMatrix(void) = default;
};

#endif 

