/**
*  @file sphere.h
*
*  @brief Defini��o de m�todos para calcular as coordenadas dos pontos de uma esfera
*         a partir de tri�ngulos.
*
*  As coordenadas s�o calculadas de maneira que, pela regra da m�o direita,
*  a superf�cie da esfera fique para fora.
*
*  @author Carlos Pereira - A61887
*  @author Jo�o Barreira  - A73831
*  @author Rafael Braga   - A61799
*/

#ifndef sphere_h
#define sphere_h


#include "primitive.h"


class Sphere : public Primitive {
	class SphereImpl;
	SphereImpl* pimpl;

public:

	/**
	* Construtor vazio.
	*/
	Sphere(void);


	/**
	* Construtor por par�metros.
	*
	* @param radius Raio da esfera
	* @param slices N�mero de slices a utilizar
	* @param stacks N�mero de stacks a utilizar
	*/
	Sphere(float radius, size_t slices = 1, size_t stacks = 1);


	/**
	* Construtor a partir de outro objecto Sphere.
	*/
	Sphere(const Sphere&);


	/**
	* Devolve o valor do raio da esfera.
	*/
	float getRadius(void);
	/**
	* Devolve o n�mero de stacks.
	*/
	size_t getStacks(void);


	/**
	* Devolve o n�mero de slices.
	*/
	size_t getSlices(void);


	/**
	* M�todo que c�lcula todos as coordenadas de todos os pontos de uma esfera.
	*
	* @return void
	*/
	void generateVertices(void);

	~Sphere(void);
};

#endif