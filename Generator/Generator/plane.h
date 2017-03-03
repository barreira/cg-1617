/**
*  @file plane.h
*
*  @brief Definição de métodos para calcular as coordenadas dos pontos de um plano
*         a partir de triângulos.
*
*  As coordenadas são calculadas de maneira que, pela regra da mão direita,
*  a superfície do plano fique voltada para cima.
*
*  @author Carlos Pereira - A61887
*  @author João Barreira  - A73831
*  @author Rafael Braga   - A61799
*
*/


#ifndef plane_h
#define plane_h


#include "primitive.h"


class Plane : public Primitive {
	class PlaneImpl;
	PlaneImpl* pimpl;

public:

	/**
	* Construtor vazio.
	*/
	Plane(void);


	/**
	* Construtor por parâmetros.
	*
	* @param dimX Dimensão do plano no eixo dos xx
	* @param dimZ Dimensão do plano no eixo dos zz
	*/
	Plane(float dimX, float dimZ);


	/**
	* Construtor a partir de outro objecto Plane.
	*/
	Plane(const Plane&);
	
	/**
	* Devolve o valor da dimensão do plano no eixo dos xx.
	*/
	float getDimX(void);


	/**
	* Devolve o valor da dimensão do plano no eixo dos zz.
	*/
	float getDimZ(void);
	

	/**
	* Método que cálcula todos as coordenadas de todos os pontos de um plano.
	*
	* @return void
	*/
	void generateVertices(void);

	~Plane(void);
};

#endif
