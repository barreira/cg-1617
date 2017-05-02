#ifndef emissiveColor_h
#define emissiveColor_h


#include "color.h"


class EmissiveColor : public Color {

public:

	EmissiveColor(void) : Color() {}


	EmissiveColor(float red, float green, float blue)
		: Color(red, blue, green) {}


	EmissiveColor(const EmissiveColor& e) : Color(e) {}


	void colorize(void)
	{
		float emis[4] = { getRed(), getGreen(), getBlue(), 1.0f };

		glMaterialfv(GL_FRONT, GL_EMISSION, emis);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	}


	~EmissiveColor(void) = default;
};


#endif

