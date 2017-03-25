#ifndef popMatrix_h
#define popMatrix_h


#include "glOperation.h"


class PopMatrix : public GLOperation {

public:
	PopMatrix(void) = default;


	void execute(void)
	{
		glPopMatrix();
	}


	~PopMatrix(void) = default;
};

#endif 

