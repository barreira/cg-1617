/**
*  @file sphere.cpp
*
*  Defini��o de m�todos para calcular as coordenadas dos pontos de uma esfera.
*
*  @author Carlos Pereira - A61887
*  @author Jo�o Barreira  - A73831
*  @author Rafael Braga   - A61799
*
*/


#include "sphere.h"
#include <math.h>


const float pi = 3.1415926f;


class Sphere::SphereImpl {
	float radius;	// Raio da esfera
	float alfa;		// �ngulo em rela��o ao eixo Y no plano XZ em radianos
	float beta;		// �ngulo em rela��o ao eixo Z no plano XY em radianos
	size_t slices;	// N�mero de slices a utilizar
	size_t stacks;	// N�mero de stacks a utilizar


public:

	/**
	* Construtor vazio.
	*/
	SphereImpl(void)
	{
		radius = 0;
		alfa = beta = 0;
		slices = stacks = 0;
	}

	
	/**
	* Construtor por par�metros.
	*
	* @param radius Raio da esfera
	* @param slices N�mero de slices a utilizar
	* @param stacks N�mero de stacks a utilizar
	*/
	SphereImpl(float radius, size_t slices, size_t stacks)
	{
		this->radius = radius;
		this->slices = slices;
		this->stacks = stacks;
	
		alfa = (2 * pi) / ((float) slices);
		beta = pi / ((float) stacks);
	}

	
	/**
	* Devolve o valor do raio.
	*/
	float getRadius(void)
	{
		return radius;
	}


	/**
	* Devolve o n�mero de slices utilizadas.
	*/
	size_t getSlices(void)
	{
		return slices;
	}


	/**
	* Devolve o n�mero de stacks utilizadas.
	*/
	size_t getStacks(void)
	{
		return stacks;
	}


	/**
	* Calcula, a partir dos dados recebidos no construtor, as coordenadas de todos
	* os pontos que definem a esfera.
	*
	* @return Vetor com as coordenadas dos pontos.
	*/
	std::vector<Vertex> generateSphere(void)
	{
		std::vector<Vertex> vertices;	// Coordenadas dos pontos a devolver
		
		float angleAlfa = 0;	// �ngulo atual no plano XZ
		float angleBeta = 0;	// �ngulo atual no plano XY


		// Pontos de refer�ncia para aplicar-se a regra da m�o direita
		Vertex vA;			//Ponto A - canto inferior esquerdo
		float xA, yA, zA;		
		
		Vertex vB;			//Ponto B - canto inferior direito
		float xB, yB, zB;		
		
		Vertex vC;			//Ponto C - canto superior direito
		float xC, yC, zC;		
		
		Vertex vD;			//Ponto D - canto superior esquerdo
		float xD, yD, zD;		

		for (size_t i = 0; i < stacks; i++) {
			angleBeta = ((float) beta * i);			//C�lculo do �ngulo Beta

			for (size_t j = 0; j < slices; j++) {
			
				angleAlfa = ((float) alfa * j);		//C�lculo do �ngulo Alfa
			
				//Ponto A - canto inferior esquerdo
				xA = radius * sin(angleBeta) * sin(angleAlfa);;
				yA = radius * cos(angleBeta);
				zA = radius * sin(angleBeta) * cos(angleAlfa);
				vA = Vertex(xA, yA, zA);

				//Ponto B - canto inferior direito
				xB = radius * sin(angleBeta) * sin(angleAlfa + alfa);;
				yB = radius * cos(angleBeta);
				zB = radius * sin(angleBeta) * cos(angleAlfa + alfa);
				vB = Vertex(xB, yB, zB);

				//Ponto C - canto superior direito
				xC = radius * sin(angleBeta + beta) * sin(angleAlfa + alfa);;
				yC = radius * cos(angleBeta + beta);
				zC = radius * sin(angleBeta + beta) * cos(angleAlfa + alfa);
				vC = Vertex(xC, yC, zC);

				//Ponto D - canto superior esquerdo
				xD = radius * sin(angleBeta + beta) * sin(angleAlfa);;
				yD = radius * cos(angleBeta + beta);
				zD = radius * sin(angleBeta + beta) * cos(angleAlfa);
				vD = Vertex(xD, yD, zD);


				//Inser��o dos pontos na estrutura com os resultados
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


/**
* Construtor por par�metros.
*
* @param radius Raio da esfera
* @param slices N�mero de slices a utilizar
* @param stacks N�mero de stacks a utilizar
*/
Sphere::Sphere(float radius, size_t slices, size_t stacks)
{
	slices = (slices < 1) ? 1 : slices;	// Por defeito, o n�mero de slices e de stacks
	stacks = (stacks < 1) ? 1 : stacks; // � sempre superior ou igual a 1

	pimpl = new SphereImpl(radius, slices, stacks);
}


/**
* Construtor a partir de outro objecto Sphere.
*/
Sphere::Sphere(const Sphere& s)
{
	pimpl = new SphereImpl(s.pimpl->getRadius(), s.pimpl->getStacks(), 
		                   s.pimpl->getSlices());
}


/**
* Devolve o valor do raio da esfera.
*/
float Sphere::getRadius(void)
{
	return pimpl->getRadius();
}


/**
* Devolve o n�mero de stacks.
*/
size_t Sphere::getStacks(void)
{
	return pimpl->getStacks();
}


/**
* Devolve o n�mero de slices.
*/
size_t Sphere::getSlices(void)
{
	return pimpl->getSlices();
}


/**
* M�todo que c�lcula todos as coordenadas de todos os pontos de uma esfera.
*
* @return void
*/
void Sphere::generateVertices(void)
{
	// Chama o m�todo que c�lcula as coordenadas da esfera
	std::vector<Vertex> vertices = pimpl->generateSphere();

	// Adiciona os v�rtices laterais ao vetor de v�rtices 
	for (size_t i = 0; i < vertices.size(); i++) {
		addVertex(vertices.at(i));
	}
}


Sphere::~Sphere(void)
{
	delete pimpl;
	pimpl = NULL;
}