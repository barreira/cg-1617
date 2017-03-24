/**
 * @file trianglesDrawing.h
 *
 * @brief Módulo que apresenta métodos para se efetuar um desenho de um
 *        conjunto de triângulos em OpenGL.
 *
 * Uma triângulo é definido como três vértices. Esta classe usa então um
 * array de vértices recorrendo à classe auxiliar Vertex. 
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
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
	class TrianglesDrawingImpl;   // Implementação da classe TrianglesDrawing
	TrianglesDrawingImpl* pimpl;  // Apontador para a implementação da classe 
	                              // TrianglesDrawing


public:

	/**
	 * Construtor por defeito.
	 */
	TrianglesDrawing(void);


	/**
	 * Construtor por parâmetros.
	 *
	 * @param vertices Conjunto de vértices a desenhar.
	 */
	TrianglesDrawing(std::vector<Vertex> vertices);


	/**
	 * Construtor de cópia.
	 *
	 * @param t Objeto da classe TrianglesDrawing a copiar.
	 */
	TrianglesDrawing(const TrianglesDrawing& t);


	/**
	 * Devolve o conjunto de vértices a desenhar.
	 */
	std::vector<Vertex> getVertices(void);
	
	
	/**
	 * Altera o conjunto de vértices a desenhar.
	 */
	void setVertices(std::vector<Vertex>);


	void compute(void);


	/**
	 * Destrutor da classe TrianglesDrawing.
	 */
	~TrianglesDrawing(void);
};


#endif
