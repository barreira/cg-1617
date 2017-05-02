#ifndef specularColor_h
#define specularColor_h


#include "color.h"


class SpecularColor : public Color {

public:

	SpecularColor(void) : Color() {}


	SpecularColor(float red, float green, float blue)
		: Color(red, blue, green) {}


	SpecularColor(const SpecularColor& s) : Color(s) {}


	void colorize(void)
	{
		float spec[4] = { getRed(), getGreen(), getBlue(), 1.0f };

		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	}


	~SpecularColor(void) = default;
};


#endif
