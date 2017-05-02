#ifndef ambientColor_h
#define ambientColor_h


#include "color.h"


class AmbientColor : public Color {

public:

	AmbientColor(void) : Color() {}


	AmbientColor(float red, float green, float blue)
		: Color(red, blue, green) {}


	AmbientColor(const AmbientColor& a) : Color(a) {}


	void colorize(void)
	{
		float ambt[4] = { getRed(), getGreen(), getBlue(), 1.0f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambt);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	}


	~AmbientColor(void) = default;
};


#endif

