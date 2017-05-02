#include <cstddef>
#include "color.h"


class Color::ColorImpl {

	float red;
	float green;
	float blue;


public:

	ColorImpl(void)
	{
		red = green = blue = 0.0f;
	}


	ColorImpl(float red, float green, float blue)
	{
		this->red = red;
		this->green = green;
		this->blue = blue;
	}


	float getRed(void)
	{
		return red;
	}


	float getGreen(void)
	{
		return green;
	}


	float getBlue(void)
	{
		return blue;
	}


	void setRed(float red)
	{
		this->red = red;
	}


	void setGreen(float green)
	{
		this->green = green;
	}


	void setBlue(float blue)
	{
		this->blue = blue;
	}


	~ColorImpl(void) = default;
};


Color::Color(void)
{
	pimpl = new ColorImpl();
}


Color::Color(float red, float green, float blue)
{
	pimpl = new ColorImpl(red, green, blue);
}


Color::Color(const Color& c)
{
	pimpl = new ColorImpl(c.pimpl->getRed(),
		                  c.pimpl->getGreen(),
		                  c.pimpl->getBlue());
}


float Color::getRed(void)
{
	return pimpl->getRed();
}


float Color::getGreen(void)
{
	return pimpl->getGreen();
}


float Color::getBlue(void)
{
	return pimpl->getBlue();
}


void Color::setRed(float red)
{
	pimpl->setRed(red);
}


void Color::setGreen(float green)
{
	pimpl->setGreen(green);
}


void Color::setBlue(float blue)
{
	pimpl->setBlue(blue);
}


Color::~Color(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}