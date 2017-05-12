/**
 * @file directionalLight.h
 *
 * @brief M�dulo que apresenta m�todos para a defini��o de uma luz direcional
 *        em OpenGL.
 *
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 12-05-2017
 */


#ifndef directionalLight_h
#define directionalLight_h


#include "light.h"


class DirectionalLight : public Light {

public:

	/**
	 * Construtor por defeito.
	 */
	DirectionalLight(void);


	/**
	 * Construtor por par�metros.
	 *
	 * @param pos       Posi��o da luz.
	 * @param diff      Cor difusa da luz.
	 * @param spec      Cor especular da luz.
	 * @param ambt      Cor ambiente da luz.
	 * @param constant  Atenua��o constante.
	 * @param linear    Atenua��o linear.
	 * @param quadratic Atenua��o quadr�tica.
	 */
	DirectionalLight(TripleFloat pos, TripleFloat diff, TripleFloat spec, TripleFloat ambt,
		             float constant, float linear, float quadratic);


	/**
	 * Construtor de c�pia.
	 *
	 * @param d Objeto da classe DirectionalLight a copiar.
	 */
	DirectionalLight(const DirectionalLight& d);


	/**
	 * Apresenta uma luz direcional em OpenGL.
	 */
	void execute(void);


	/**
	 * Destrutor da classe DirectionalLight.
	 */
	~DirectionalLight(void);
};


#endif 

