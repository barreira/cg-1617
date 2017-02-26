#include "sphere.h"
#include <math.h>

const float pi = 3.1415926;

class Sphere::SphereImpl {
	float radius;
	double alfa;	//ângulo na oriem no plano XZ
	double beta;	//ângulo na origem no plano XY
	float stackHeight;	//altura duma stack
	float sliceWidth;	//largura duma stack
	size_t slices;
	size_t stacks;


public:
	SphereImpl(void)
	{
		radius = 0;
		alfa = beta = 0;
		slices = stacks = 0;
	}

	
	SphereImpl(float radius, size_t slices, size_t stacks)
	{
		this->radius = radius;
		this->slices = slices;
		this->stacks = stacks;
	
		alfa = (2 * pi) / ((double) slices);
		beta = (2 * pi) / ((double) stacks);
	}

	
	float getRadius(void)
	{
		return radius;
	}

	double getAlfa(void)
	{
		return alfa;
	}

	double getBeta(void)
	{
		return beta;
	}

	size_t getSlices(void)
	{
		return slices;
	}


	size_t getStacks(void)
	{
		return stacks;
	}


	std::vector<Vertex*> generateTopAndBottom(void)
	{
		std::vector<Vertex*> vertices;
		double angle = 0;




	}

	~SphereImpl(void) = default;
};


Sphere::Sphere(void) : pimpl{ new SphereImpl() } {}

Sphere::Sphere(float radius, size_t slices, size_t stacks)
{
	slices = (slices < 1) ? 1 : slices;
	stacks = (stacks < 1) ? 1 : stacks;

	pimpl = std::unique_ptr<SphereImpl>(new SphereImpl(radius, slices, stacks));
}

float Sphere::getRadius(void)
{
	return pimpl->getRadius();
}


double Sphere::getAlfa(void)
{
	return pimpl->getAlfa();
}

double Sphere::getBeta(void)
{
	return pimpl->getBeta();
}


size_t Sphere::getStacks(void)
{
	return pimpl->getStacks();
}

size_t Sphere::getSlices(void)
{
	return pimpl->getSlices();
}

void Sphere::generateVertices(void)
{


}

Sphere::~Sphere(void) = default;