/**
*  @file cone.h
*
*  @brief Defini��o de m�todos para calcular as coordenadas dos pontos de um cone 
*         a partir de tri�ngulos.
*  
*  As coordenadas s�o calculadas de maneira que, pela regra da m�o direita,
*  todas as superf�cies (lateral e base) fiquem para fora.
*
*  @author Carlos Pereira - A61887
*  @author Jo�o Barreira  - A73831
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
	* Construtor por par�metros.
	*
	* @param radius Raio da base do cone
	* @param height Altura do cone
	* @param slices N�mero de slices a utilizar
	* @param stacks N�mero de stacks a utilizar
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
	* Devolve o n�mero de stacks.
	*/
	size_t getStacks(void);


	/**
	* Devolve o n�mero de slices.
	*/
	size_t getSlices(void);


	/**
	* M�todo que c�lcula todos as coordenadas de todos os pontos de um cone.
	*
	* @return void
	*/
	void generateVertices(void);

	~Cone(void);
};

#endif