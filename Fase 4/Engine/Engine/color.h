#ifndef color_h
#define color_h


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class Color {
	class ColorImpl;
	ColorImpl* pimpl;


public:



	Color(void);


	Color(float red, float green, float blue);


	Color(const Color& c);


	float getRed(void);


	float getGreen(void);


	float getBlue(void);


	void setRed(float);


	void setGreen(float);


	void setBlue(float);


	virtual void colorize(void) = 0;


	virtual ~Color(void);
};


#endif
