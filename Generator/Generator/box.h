/**
*  @file box.h
*
*  @brief Definição de métodos para calcular as coordenadas dos pontos de uma caixa
*         (cubo ou paralelipípedo) a partir de triângulos.
*
*  As coordenadas são calculadas de maneira que, pela regra da mão direita,
*  as faces da caixa fiquem voltadas para fora.
*
*  @author Carlos Pereira - A61887
*  @author João Barreira  - A73831
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
	* Construtor por parâmetros.
	*
	* @param dimX   Dimensão do plano no eixo dos xx
	* @param dimY   Dimensão do plano no eixo dos yy
	* @param dimZ   Dimensão do plano no eixo dos zz
	* @param numDiv Número de divisões da caixa
	*/
	Box(float dimX, float dimY, float dimZ, size_t numDiv = 0);


	/**
	* Construtor a partir de outro objecto Box.
	*/
	Box(const Box&);


	/**
	* Devolve o valor da dimensão da caixa no eixo dos xx.
	*/
	float getDimX(void);


	/**
	* Devolve o valor da dimensão da caixa no eixo dos yy.
	*/
	float getDimY(void);


	/**
	* Devolve o valor da dimensão da caixa no eixo dos zz.
	*/
	float getDimZ(void);


	/**
	* Devolve o número de divisões da caixa.
	*/
	size_t getNumDiv(void);


	/**
	* Método que cálcula todos as coordenadas de todos os pontos de uma caixa.
	*
	* @return void
	*/
	void generateVertices(void);

	~Box(void);
};

#endif
