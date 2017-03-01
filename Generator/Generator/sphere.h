/**
*  @file sphere.h
*
*  @brief Definição de métodos para calcular as coordenadas dos pontos de uma esfera
*         a partir de triângulos.
*
*  As coordenadas são calculadas de maneira que, pela regra da mão direita,
*  a superfície da esfera fique para fora.
*
*  @author Carlos Pereira - A61887
*  @author João Barreira  - A73831
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
	* Construtor por parâmetros.
	*
	* @param radius Raio da esfera
	* @param slices Número de slices a utilizar
	* @param stacks Número de stacks a utilizar
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
	* Devolve o número de stacks.
	*/
	size_t getStacks(void);


	/**
	* Devolve o número de slices.
	*/
	size_t getSlices(void);


	/**
	* Método que cálcula todos as coordenadas de todos os pontos de uma esfera.
	*
	* @return void
	*/
	void generateVertices(void);

	~Sphere(void);
};

#endif