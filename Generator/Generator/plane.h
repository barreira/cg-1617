/**
*  @file plane.h
*
*  @brief Defini��o de m�todos para calcular as coordenadas dos pontos de um plano
*         a partir de tri�ngulos.
*
*  As coordenadas s�o calculadas de maneira que, pela regra da m�o direita,
*  a superf�cie do plano fique voltada para cima.
*
*  @author Carlos Pereira - A61887
*  @author Jo�o Barreira  - A73831
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
	* Construtor por par�metros.
	*
	* @param dimX Dimens�o do plano no eixo dos xx
	* @param dimZ Dimens�o do plano no eixo dos zz
	*/
	Plane(float dimX, float dimZ);


	/**
	* Construtor a partir de outro objecto Plane.
	*/
	Plane(const Plane&);
	
	/**
	* Devolve o valor da dimens�o do plano no eixo dos xx.
	*/
	float getDimX(void);


	/**
	* Devolve o valor da dimens�o do plano no eixo dos zz.
	*/
	float getDimZ(void);
	

	/**
	* M�todo que c�lcula todos as coordenadas de todos os pontos de um plano.
	*
	* @return void
	*/
	void generateVertices(void);

	~Plane(void);
};

#endif
