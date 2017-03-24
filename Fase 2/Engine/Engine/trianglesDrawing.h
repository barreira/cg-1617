/**
 * @file trianglesDrawing.h
 *
 * @brief M�dulo que apresenta m�todos para se efetuar um desenho de um
 *        conjunto de tri�ngulos em OpenGL.
 *
 * Uma tri�ngulo � definido como tr�s v�rtices. Esta classe usa ent�o um
 * array de v�rtices recorrendo � classe auxiliar Vertex. 
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 18-03-2017
 * 
 * TODO: Arranjar um nome melhor para esta classe 
 */


#ifndef trianglesDrawing_h
#define trianglesDrawing_h


#include <vector>
#include "operation.h"
#include "vertex.h"


class TrianglesDrawing : public Operation {
	class TrianglesDrawingImpl;   // Implementa��o da classe TrianglesDrawing
	TrianglesDrawingImpl* pimpl;  // Apontador para a implementa��o da classe 
	                              // TrianglesDrawing


public:

	/**
	 * Construtor por defeito.
	 */
	TrianglesDrawing(void);


	/**
	 * Construtor por par�metros.
	 *
	 * @param vertices Conjunto de v�rtices a desenhar.
	 */
	TrianglesDrawing(std::vector<Vertex> vertices);


	/**
	 * Construtor de c�pia.
	 *
	 * @param t Objeto da classe TrianglesDrawing a copiar.
	 */
	TrianglesDrawing(const TrianglesDrawing& t);


	/**
	 * Devolve o conjunto de v�rtices a desenhar.
	 */
	std::vector<Vertex> getVertices(void);
	
	
	/**
	 * Altera o conjunto de v�rtices a desenhar.
	 */
	void setVertices(std::vector<Vertex>);


	void compute(void);


	/**
	 * Destrutor da classe TrianglesDrawing.
	 */
	~TrianglesDrawing(void);
};


#endif
