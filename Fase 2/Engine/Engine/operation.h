/**
 * @file operation.h
 *
 * Módulo que representa a classe base para uma operação qualquer em
 * OpenGL (translação, rotação, desenho de primitivas, etc).
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 18-03-2017
 *
 * TODO: Arranjar um nome melhor para esta classe. Se calhar todas a classes 
 *       derivadas podiam implementar um método abstrato (compute por exemplo)
 *       em que efetuavam uma operação qualquer.
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


	// Possibilidade do método abstrato
	virtual void compute(void) = 0;


	/**
	 * Destrutor da classe Operation (por defeito em c++). Todas as classes
	 * derivadas desta classe implementam as suas versões deste destrutor.
	 */
	virtual ~Operation(void) = default;
};


#endif
