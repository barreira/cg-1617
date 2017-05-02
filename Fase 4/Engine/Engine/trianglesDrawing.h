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
 * @version 02-05-2017
 */


#ifndef trianglesDrawing_h
#define trianglesDrawing_h


#include <vector>
#include "glOperation.h"
#include "vertex.h"
#include "color.h"


class TrianglesDrawing : public GLOperation {
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
	 * @param normals  Conjunto de normais de uma primitiva.
	 * @param indexes  Conjunto de �ndices.
	 * @param color    Cor da primitiva
	 */
	TrianglesDrawing(std::vector<GLfloat> vertices, 
		             std::vector<GLfloat> normals,
		             std::vector<size_t> indexes,
		             Color* color);


	/**
	 * Construtor de c�pia.
	 *
	 * @param t Objeto da classe TrianglesDrawing a copiar.
	 */
	TrianglesDrawing(const TrianglesDrawing& t);


	/**
	 * Devolve o conjunto de v�rtices a desenhar.
	 */
	std::vector<GLfloat> getVertices(void);
	

	/**
	 * Devolve o conjunto de normais.
	 */
	std::vector<GLfloat> getNormals(void);


	/**
	 * Devolve o conjunto de �ndices associados ao vetor de v�rtices.
	 */
	std::vector<size_t> getIndexes(void);

	
	/**
	 * Devolve a cor do modelo.
	 */
	Color* getColor(void);


	/**
	 * Altera o conjunto de v�rtices a desenhar.
	 */
	void setVertices(std::vector<GLfloat>);


	/**
	 * Altera o conjunto de normais.
	 */
	void setNormals(std::vector<GLfloat>);


	/**
	 * Altera o conjunto de �ndices associados ao vetor de v�rtices.
	 */
	void setIndexes(std::vector<size_t>);


	/**
	 * Altera a cor do modelo.
	 */
	void setColor(Color*);


	/**
	 * Desenha um conjunto de v�rtices de uma primitiva em OpenGL.
	 */
	void execute(void);


	/**
	 * Destrutor da classe TrianglesDrawing.
	 */
	~TrianglesDrawing(void);
};


#endif
