/**
 * @file sphere.cpp
 *
 * Defini��o de m�todos para calcular as coordenadas dos pontos de uma esfera.
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 * 
 * @version 12-05-2017
 */


#include "sphere.h"
#include <cmath>


const float PI = 3.1415926f;


class Sphere::SphereImpl {
	float radius;	// Raio da esfera
	float alpha;    // �ngulo em rela��o ao eixo Y no plano XZ em radianos
	float beta;		// �ngulo em rela��o ao eixo Z no plano XY em radianos
	int slices;	    // N�mero de slices a utilizar
	int stacks;	    // N�mero de stacks a utilizar


public:

	/**
	 * Construtor por defeito.
	 */
	SphereImpl(void)
	{
		radius = 0.0f;
		alpha = beta = 0.0f;
		slices = stacks = 0;
	}

	
	/**
	 * Construtor por par�metros.
	 *
	 * @param radius Raio da esfera
	 * @param slices N�mero de slices a utilizar
	 * @param stacks N�mero de stacks a utilizar
	 */
	SphereImpl(float radius, int slices, int stacks)
	{
		this->radius = radius;
		this->slices = slices;
		this->stacks = stacks;
	
		alpha = (2.0f * PI) / ((float) slices);
		beta = PI / ((float) stacks);
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
	int getSlices(void)
	{
		return slices;
	}


	/**
	 * Devolve o n�mero de stacks utilizadas.
	 */
	int getStacks(void)
	{
		return stacks;
	}


	/**
	 * Altera o valor do raio.
	 */
	void setRadius(float radius)
	{
		this->radius = radius;
	}


	/**
	 * Altera o n�mero de stacks.
	 */
	void setStacks(int stacks)
	{
		this->stacks = stacks;

		beta = PI / ((float)stacks);
	}


	/**
	 * Altera o n�mero de slices.
	 */
	void setSlices(int slices)
	{
		this->slices = slices;

		alpha = (2.0f * PI) / ((float)slices);
	}


	/**
	 * Calcula, a partir dos dados recebidos no construtor, as coordenadas de 
	 * todos os v�rtices que definem a esfera.
	 *
	 * @param vertices  Conjunto de v�rtices.
	 * @param normals   Conjunto de normais.
	 * @param texCoords Conjunto de coordenadas de uma textura.
	 * @param indexes   Conjunto de �ndices.
	 * @return O resultado � guardado em vertices e em indexes.
	 */
	void generateSphere(std::vector<TripleFloat>& vertices,
		                std::vector<TripleFloat>& normals,
		                std::vector<TripleFloat>& texCoords,
		                std::vector<size_t>& indexes)
	{
		float angleAlpha = 0.0f; // �ngulo atual no plano XZ
		float angleBeta = 0.0f;	 // �ngulo atual no plano XY

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float nx = 0.0f;
		float ny = 0.0f;
		float nz = 0.0f;

		size_t index = 0;       // �ndice atual do conjunto de �ndices

		for (size_t i = 0; i <= (size_t)stacks; i++) {
			
			//C�lculo do �ngulo Beta
			angleBeta = ((float) beta * i);			

			for (size_t j = 0; j <= (size_t)slices; j++) {
				
				//C�lculo do �ngulo Alpha
				angleAlpha = ((float) alpha * j);

				x = radius * sin(angleBeta) * cos(angleAlpha);
				y = radius * cos(angleBeta);
				z = radius * sin(angleBeta) * sin(angleAlpha);

				vertices.push_back(TripleFloat(x, y, z));

				nx = sin(angleBeta) * cos(angleAlpha);
				ny = cos(angleBeta);
				nz = sin(angleBeta) * sin(angleAlpha);

				normals.push_back(TripleFloat(nx, ny, nz));


				texCoords.push_back(TripleFloat((slices - (float)j) / slices, 
					                            (stacks - (float)i) / stacks, 
					                            0.0f));
			}
		}

		for (size_t i = 0; i < (size_t)stacks; i++) {
			for (size_t j = 0; j < (size_t)slices; j++) {
				indexes.push_back(i * (slices + 1) + j);
				indexes.push_back(i * (slices + 1) + j + 1);
				indexes.push_back((i + 1) * (slices + 1) + j + 1);

				indexes.push_back(i * (slices + 1) + j);
				indexes.push_back((i + 1) * (slices + 1) + j + 1);
				indexes.push_back((i + 1) * (slices + 1) + j);
			}
		}
	}


	~SphereImpl(void) = default;
};


/**
 * Construtor por defeito.
 */
Sphere::Sphere(void) {
	pimpl = new SphereImpl();
} 


/**
 * Construtor por par�metros.
 *
 * @param radius Raio da esfera
 * @param slices N�mero de slices a utilizar
 * @param stacks N�mero de stacks a utilizar
 */
Sphere::Sphere(float radius, int slices, int stacks)
{
	// Por defeito, o n�mero de slices e stacks � sempre maior ou igual a 1

	slices = (slices < 1) ? 1 : slices;	
	stacks = (stacks < 1) ? 1 : stacks; 

	// O raio n�o pode conter um valor negativo

	radius = (radius < 0.0f) ? 0.0f : radius;

	pimpl = new SphereImpl(radius, slices, stacks);

	generateVertices();
}


/**
 * Construtor de c�pia.
 *
 * @param s Objeto de uma Sphere a ser copiado.
 */
Sphere::Sphere(const Sphere& s)
{
	pimpl = new SphereImpl(s.pimpl->getRadius(), s.pimpl->getSlices(), 
		                   s.pimpl->getStacks());
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
int Sphere::getStacks(void)
{
	return pimpl->getStacks();
}


/**
 * Devolve o n�mero de slices.
 */
int Sphere::getSlices(void)
{
	return pimpl->getSlices();
}


/**
 * Altera o valor do raio.
 */
void Sphere::setRadius(float radius)
{
	radius = (radius < 0.0f) ? 0.0f : radius;
	pimpl->setRadius(radius);
}


/**
 * Altera o n�mero de stacks.
 */
void Sphere::setStacks(int stacks)
{
	stacks = (stacks < 1) ? 1 : stacks;
	pimpl->setStacks(stacks);
}


/**
 * Altera o n�mero de slices.
 */
void Sphere::setSlices(int slices)
{
	slices = (slices < 1) ? 1 : slices;
	pimpl->setSlices(slices);
}


/**
 * M�todo que c�lcula todos as coordenadas de todos os pontos de uma esfera.
 */
void Sphere::generateVertices(void)
{
	std::vector<TripleFloat> vertices;
	std::vector<TripleFloat> normals;
	std::vector<TripleFloat> texCoords;
	std::vector<size_t> indexes;

	pimpl->generateSphere(vertices, normals, texCoords, indexes);

	for (size_t i = 0; i < vertices.size(); i++) {
		addVertex(vertices.at(i));
		addNormal(normals.at(i));
		addTexCoord(texCoords.at(i));
	}

	for (size_t i = 0; i < indexes.size(); i++) {
		addIndex(indexes.at(i));
	}
}


/**
 * Destrutor da classe Sphere.
 *
 * Liberta a mem�ria ocupada pelo apontador para a parte privada da classe
 * Sphere.
 */
Sphere::~Sphere(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}