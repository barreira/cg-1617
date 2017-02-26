#include "sphere.h"
#include <math.h>


const float pi = 3.1415926f;


class Sphere::SphereImpl {
	float radius;
	float alfa;	//ângulo em relação ao eixo Y no plano XZ
	float beta;	//ângulo em relação ao eixo Z no plano XY
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
	
		alfa = (2 * pi) / ((float) slices);
		beta = pi / ((float) stacks);
	}

	
	float getRadius(void)
	{
		return radius;
	}


	size_t getSlices(void)
	{
		return slices;
	}


	size_t getStacks(void)
	{
		return stacks;
	}


	std::vector<Vertex> generateSphere(void)
	{
		std::vector<Vertex> vertices;
		
		float angleAlfa = 0;	//ângulo atual no plano XZ
		float angleBeta = 0;
		for (size_t i = 0; i < stacks; i++) {
			angleBeta = ((float) beta * i);

			for (size_t j = 0; j < slices; j++) {
				angleAlfa = ((float) alfa * j);
			
				//Ponto A - canto inferior esquerdo
				float xA = radius * sin(angleBeta) * sin(angleAlfa);;
				float yA = radius * cos(angleBeta);
				float zA = radius * sin(angleBeta) * cos(angleAlfa);
				Vertex vA = Vertex(xA, yA, zA);

				
				//Ponto B - canto inferior direito
				float xB = radius * sin(angleBeta) * sin(angleAlfa + alfa);;
				float yB = radius * cos(angleBeta);
				float zB = radius * sin(angleBeta) * cos(angleAlfa + alfa);
				Vertex vB = Vertex(xB, yB, zB);

				
				//Ponto C - canto superior direito
				float xC = radius * sin(angleBeta + beta) * sin(angleAlfa + alfa);;
				float yC = radius * cos(angleBeta + beta);
				float zC = radius * sin(angleBeta + beta) * cos(angleAlfa + alfa);
				Vertex vC = Vertex(xC, yC, zC);


				//Ponto D - canto superior esquerdo
				float xD = radius * sin(angleBeta + beta) * sin(angleAlfa);;
				float yD = radius * cos(angleBeta + beta);
				float zD = radius * sin(angleBeta + beta) * cos(angleAlfa);
				Vertex vD = Vertex(xD, yD, zD);

				vertices.push_back(vA);
				vertices.push_back(vB);
				vertices.push_back(vC);

				vertices.push_back(vA);
				vertices.push_back(vC);
				vertices.push_back(vD);
			}
		}

		return vertices;
	}


	~SphereImpl(void) = default;
};


Sphere::Sphere(void) : pimpl{ new SphereImpl() } {}


Sphere::Sphere(float radius, size_t slices, size_t stacks)
{
	slices = (slices < 1) ? 1 : slices;
	stacks = (stacks < 1) ? 1 : stacks;

	pimpl = new SphereImpl(radius, slices, stacks);
}


Sphere::Sphere(const Sphere& s)
{
	pimpl = new SphereImpl(s.pimpl->getRadius(), s.pimpl->getStacks(), 
		                   s.pimpl->getSlices());
}


float Sphere::getRadius(void)
{
	return pimpl->getRadius();
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
	std::vector<Vertex> vertices = pimpl->generateSphere();

	for (size_t i = 0; i < vertices.size(); i++) {
		addVertex(vertices.at(i));
	}
}


Sphere::~Sphere(void)
{
	delete pimpl;
	pimpl = NULL;
}