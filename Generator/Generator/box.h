/**
*  @file box.h
*
*  @brief Defini��o de m�todos para calcular as coordenadas dos pontos de uma caixa
*         (cubo ou paralelip�pedo) a partir de tri�ngulos.
*
*  As coordenadas s�o calculadas de maneira que, pela regra da m�o direita,
*  as faces da caixa fiquem voltadas para fora.
*
*  @author Carlos Pereira - A61887
*  @author Jo�o Barreira  - A73831
*  @author Rafael Braga   - A61799
*/


#ifndef box_h
#define box_h


#include "primitive.h"


class Box : public Primitive {
	class BoxImpl;
	BoxImpl* pimpl;

public:

	/**
	* Construtor vazio.
	*/
	Box(void);


	/**
	* Construtor por par�metros.
	*
	* @param dimX   Dimens�o do plano no eixo dos xx
	* @param dimY   Dimens�o do plano no eixo dos yy
	* @param dimZ   Dimens�o do plano no eixo dos zz
	* @param numDiv N�mero de divis�es da caixa
	*/
	Box(float dimX, float dimY, float dimZ, size_t numDiv = 0);


	/**
	* Construtor a partir de outro objecto Box.
	*/
	Box(const Box&);


	/**
	* Devolve o valor da dimens�o da caixa no eixo dos xx.
	*/
	float getDimX(void);


	/**
	* Devolve o valor da dimens�o da caixa no eixo dos yy.
	*/
	float getDimY(void);


	/**
	* Devolve o valor da dimens�o da caixa no eixo dos zz.
	*/
	float getDimZ(void);


	/**
	* Devolve o n�mero de divis�es da caixa.
	*/
	size_t getNumDiv(void);


	/**
	* M�todo que c�lcula todos as coordenadas de todos os pontos de uma caixa.
	*
	* @return void
	*/
	void generateVertices(void);

	~Box(void);
};

#endif
