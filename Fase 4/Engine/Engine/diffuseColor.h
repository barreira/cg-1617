#ifndef diffuseColor_h
#define diffuseColor_h


#include "color.h"


class DiffuseColor : public Color {

public:

	DiffuseColor(void) : Color() {}


	DiffuseColor(float red, float green, float blue) 
		: Color(red, blue, green) {}


	DiffuseColor(const DiffuseColor& d) : Color(d) {}


	void colorize(void)
	{
		float diff[4] = { getRed(), getGreen(), getBlue(), 1.0f };

		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	}


	~DiffuseColor(void) = default;
};


#endif
