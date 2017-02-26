#include "cone.h"
#include <math.h>

const float pi = 3.1415926;

class Cone::ConeImpl {
	float radius;
	float height;
	double alfa;		//ângulo ao centro em radianos
	float stackHeight;	//altura de uma stack
	size_t slices;
	size_t stacks;


public:
	ConeImpl(void)
	{
		radius = height = stackHeight = 0;
		alfa = 0;
		slices = stacks = 0;
	}

	ConeImpl(float radius, float height, size_t slices, size_t stacks)
	{
		this->radius = radius;
		this->height = height;
		this->slices = slices;
		this->stacks = stacks;
		alfa = (2 * pi) / ((double) slices);
		stackHeight = height / ((float) stacks);
	}


	float getRadius(void)
	{
		return radius;
	}


	float getHeight(void)
	{
		return height;
	}


	double getAlfa(void)
	{
		return alfa;
	}


	float getStackHeight(void)
	{
		return stackHeight;
	}


	size_t getStacks(void)
	{
		return stacks;
	}

	
	size_t getSlices(void)
	{
		return slices;
	}


	std::vector<Vertex*> generateBase(void)
	{
		std::vector<Vertex*> vertices;
		double angle = 0;
		float x = 0;
		float y = 0;
		float z = radius;
		Vertex* center = new Vertex(0, 0, 0);


		for (size_t i = 0; i < slices; i++) {
			vertices.push_back(new Vertex(x, y, z));
			
			vertices.push_back(center);
			
			angle = ((double) (alfa * (i + 1)));

			x = ((float) radius * sin(angle));
			z = ((float) radius * cos(angle));
			vertices.push_back(new Vertex(x, y, z));

		}

		return vertices;
	}


	std::vector<Vertex*> generateSides(void)
	{
		std::vector<Vertex*> vertices;
		double angle = 0;		//ângulo atual

		//coordenadas de referência
		float lowerR = 0;	//"raio" do círculo dos 2 pontos inferiores
		float upperR = 0;	//"raio" do círculo dos 2 pontos superiores
		float lowerH = 0;	//coordenada em Y dos 2 pontos inferiores
		float upperH = 0;	//coordenada em Y dos 2 pontos superiores


		for (size_t i = 0; i < stacks; i++) {
			lowerH = i * stackHeight - height/4;		//Ajustar a altura 
			upperH = (i + 1) * stackHeight - height/4;	//ao centro de massa

			lowerR = radius - radius * lowerH / height;
			upperR = radius - radius * upperH / height;


			for (size_t j = 0; j < slices; j++) {
				angle = ((double)(alfa * j));


				//Ponto A - canto inferior esquerdo
				float xA = ((float)lowerR * sin(angle));
				float yA = lowerH;
				float zA = ((float)lowerR * cos(angle));

				//Ponto B -  canto inferior direito
				float xB = ((float)lowerR * sin(angle + alfa));
				float yB = lowerH;
				float zB = ((float)lowerR * cos(angle + alfa));

				//Ponto C - canto superior direito
				float xC = ((float)upperR * sin(angle + alfa));
				float yC = upperH;
				float zC = ((float)upperR * cos(angle + alfa));

				//Ponto D - canto superior esquerdo
				float xD = ((float)upperR * sin(angle));
				float yD = upperH;
				float zD = ((float)upperR * cos(angle));

				vertices.push_back(new Vertex(xA, yA, zA));
				vertices.push_back(new Vertex(xB, yB, zB));		
				vertices.push_back(new Vertex(xC, yC, zC));		

				vertices.push_back(new Vertex(xA, yA, zA));
				vertices.push_back(new Vertex(xC, yC, zC));
				vertices.push_back(new Vertex(xD, yD, zD));
			}
			
		}
	
		return vertices;
	}

	~ConeImpl(void) = default;
};


Cone::Cone(void) : pimpl{ new ConeImpl() } {}

Cone::Cone(float radius, float height, size_t slices, size_t stacks)
{
	slices = (slices < 1) ? 1 : slices;
	stacks = (stacks < 1) ? 1 : stacks;


	pimpl = std::unique_ptr<ConeImpl>(new ConeImpl(radius, height, slices, stacks));
}

float Cone::getRadius(void)
{
	return pimpl->getRadius();
}


float Cone::getHeight(void)
{
	return pimpl->getHeight();
}


double Cone::getAlfa(void)
{
	return pimpl->getAlfa();
}


float Cone::getStackHeight(void)
{
	return pimpl->getStackHeight();
}


size_t Cone::getStacks(void)
{
	return pimpl->getStacks();
}


size_t Cone::getSlices(void)
{
	return pimpl->getSlices();
}

void Cone::generateVertices(void)
{
	std::vector<Vertex*> verticesBase = pimpl->generateBase();
	std::vector<Vertex*> verticesSides = pimpl->generateSides();

	for (size_t i = 0; i < verticesBase.size(); i++) {
		addVertex(verticesBase.at(i));
	}

	for (size_t i = 0; i < verticesSides.size(); i++) {
		addVertex(verticesSides.at(i));
	}

}



Cone::~Cone(void) = default;