/**
*  @file cone.h
*
*  @brief Definição de métodos para calcular as coordenadas dos pontos de um cone 
*         a partir de triângulos.
*  
*  As coordenadas são calculadas de maneira que, pela regra da mão direita,
*  todas as superfícies (lateral e base) fiquem para fora.
*
*  @author Carlos Pereira - A61887
*  @author João Barreira  - A73831
*  @author Rafael Braga   - A61799
*
*/


#ifndef cone_h
#define cone_h


#include "primitive.h"


class Cone : public Primitive {
	class ConeImpl;
	ConeImpl* pimpl;

public:

	/**
	* Construtor vazio.
	*/
	Cone(void);


	/**
	* Construtor por parâmetros.
	*
	* @param radius Raio da base do cone
	* @param height Altura do cone
	* @param slices Número de slices a utilizar
	* @param stacks Número de stacks a utilizar
	*/
	Cone(float radius, float height, size_t slices = 1, size_t stacks = 1);
	

	/**
	* Construtor a partir de outro objecto Cone.
	*/
	Cone(const Cone&);


	/**
	* Devolve o valor do raio da base.
	*/
	float getRadius(void);
	
	
	/**
	* Devolve a altura do cone.
	*/
	float getHeight(void);


	/**
	* Devolve o número de stacks.
	*/
	size_t getStacks(void);


	/**
	* Devolve o número de slices.
	*/
	size_t getSlices(void);


	/**
	* Método que cálcula todos as coordenadas de todos os pontos de um cone.
	*
	* @return void
	*/
	void generateVertices(void);

	~Cone(void);
};

#endif