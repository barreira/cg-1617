/**
 * @file primitive.h
 *
 * @brief Módulo que representa a classe abstrata Primitive. Uma primitiva pode
 *        ser vista como um conjunto de vértices.
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 02-05-2017
 */


#ifndef primitive_h
#define primitive_h

#include <vector>
#include "vertex.h"

class Primitive {
	class PrimitiveImpl;
	PrimitiveImpl* pimpl;

public:

	/**
	 * Construtor por defeito.
	 */
	Primitive(void);


	/**
	 * Construtor por parâmetros.
	 *
	 * @param vertices Conjunto de vértices de uma primitiva.
	 * @param normals  Conjunto de normais de uma primitiva.
	 * @param indexes  Conjunto de índices associados ao vetor de vértices.
	 */
	Primitive(std::vector<Vertex> vertices,
		      std::vector<Vertex> normals,
		      std::vector<size_t> indexes);


	/**
	 * Construtor de cópia.
	 *
	 * @param p Objeto da classe Primitive a ser copiado.
	 */
	Primitive(const Primitive& p);
	

	/**
	 * Devolve o conjunto de vértices que definem uma primitiva.
	 */
	std::vector<Vertex> getVertices(void);


	/**
	 * Devolve o conjunto de normais que definem uma primitiva.
	 */
	std::vector<Vertex> getNormals(void);


	/**
	 * Devolve o conjunto de índices associados ao vetor de vértices.
	 */
	std::vector<size_t> getIndexes(void);


	/**
	 * Adiciona um vértice ao conjunto de vértices de uma primitiva.
	 *
	 * @param v Vértice a adicionar.
	 */
	void addVertex(Vertex v);


	/**
	 * Adiciona uma normal ao conjunto de normais de uma primitiva.
	 *
	 * @param n Normal a adicionar.
	 */
	void addNormal(Vertex n);


	/**
	 * Adiciona um índice ao conjunto de índices.
	 *
	 * @param index Índice a adicionar.
	 */
	void addIndex(size_t index);


	/**
	 * Método abstrato para gerar os vértices de um tipo de primitiva 
	 * qualquer.
	 */
	virtual void generateVertices(void) = 0;


	/**
	 * Destrutor. 
	 */
	virtual ~Primitive(void);
};

#endif 

