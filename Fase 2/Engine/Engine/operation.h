/**
 * @file operation.h
 *
 * M�dulo que representa a classe base para uma opera��o qualquer em
 * OpenGL (transla��o, rota��o, desenho de primitivas, etc).
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 18-03-2017
 *
 * TODO: Arranjar um nome melhor para esta classe. Se calhar todas a classes 
 *       derivadas podiam implementar um m�todo abstrato (compute por exemplo)
 *       em que efetuavam uma opera��o qualquer.
 */


#ifndef operation_h
#define operation_h


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class Operation {

public:

	/**
	 * Construtor da classe Operation (por defeito em c++).
	 */
	Operation(void) = default;


	// Possibilidade do m�todo abstrato
	virtual void compute(void) = 0;


	/**
	 * Destrutor da classe Operation (por defeito em c++). Todas as classes
	 * derivadas desta classe implementam as suas vers�es deste destrutor.
	 */
	virtual ~Operation(void) = default;
};


#endif
