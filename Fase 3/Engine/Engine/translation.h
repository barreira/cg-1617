/**
 * @file translation.h
 *
 * @brief M�dulo que apresenta m�todos para se efetuar uma transla��o em OpenGL.
 *
 * Uma transla��o pode ser definida � custa dos valores de x, y e z.
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 12-04-2017
 */


#ifndef translation_h
#define translation_h


#include <vector>
#include "glOperation.h"
#include "vertex.h"


class Translation : public GLOperation {
	class TranslationImpl;   // Implementa��o da classe Translation
	TranslationImpl* pimpl;  // Apontador para a implementa��o da classe 
	                         // Translation

public:

	/**
	 * Construtor por defeito.
	 */
	Translation(void);


	/**
	 * Construtor por par�metros.
	 *
	 * @param x Valor de uma transla��o em x.
	 * @param y Valor de uma transla��o em y.
	 * @param z Valor de uma transla��o em z.
	 */
	Translation(float x, float y, float z);


	/**
	 * Construtor por par�metros.
	 *
	 * @param x Valor de uma transla��o em x.
	 * @param y Valor de uma transla��o em y.
	 * @param z Valor de uma transla��o em z.
	 */
	Translation(float x, float y, float z,
		        std::vector<Vertex> catmullPoints, float totalTime);


	/**
	 * Construtor de c�pia.
	 *
	 * @param t Objeto da classe Translation a copiar.
	 */
	Translation(const Translation& t);


	/**
	 * Devolve o valor de uma transla��o em x.
	 */
	float getX(void);


	/**
	 * Devolve o valor de uma transla��o em y.
	 */
	float getY(void);


	/**
	 * Devolve o valor de uma transla��o em z.
	 */
	float getZ(void);


	std::vector<Vertex> getCatmullPoints(void);


	float getTotalTime(void);


	/**
	 * Altera o valor de uma transla��o em x.
	 */
	void setX(float);


	/**
	 * Altera o valor de uma transla��o em y.
	 */
	void setY(float);


	/**
	 * Altera o valor de uma transla��o em z.
	 */
	void setZ(float);


	void setCatmullPoints(std::vector<Vertex>);


	void setTotalTime(float);


	/**
	 * Executa uma transla��o em x, y ou z em OpenGL.
	 */
	void execute(void);


	/**
	 * Destrutor da classe Translation.
	 */
	~Translation(void);
};


#endif

