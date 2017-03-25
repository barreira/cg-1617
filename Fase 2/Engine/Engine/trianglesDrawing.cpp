/**
 * @file trianglesDrawing.cpp
 *
 * Definição de todos os métodos para se representar um conjunto de vértices em
 * OpenGL.
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 18-03-2017
 */


#include "trianglesDrawing.h"


class TrianglesDrawing::TrianglesDrawingImpl {
	std::vector<Vertex> vertices;  // Conjunto de vértices a desenhar


public:

	/**
	 * Construtor por defeito.
	 */
	TrianglesDrawingImpl(void) = default;


	/**
	 * Construtor por parâmetros.
	 *
	 * @param vertices Conjunto de vértices a desenhar.
	 */
	TrianglesDrawingImpl(std::vector<Vertex> vertices)
	{
		for (size_t i = 0; i < vertices.size(); i++) {
			this->vertices.push_back(vertices.at(i));
		}
	}


	/**
	 * Devolve o conjunto de vértices a desenhar.
	 */
	std::vector<Vertex> getVertices(void)
	{
		return vertices;
	}


	/**
	 * Altera o conjunto de vértices a desenhar.
	 */
	void setVertices(std::vector<Vertex> vertices)
	{
		for (size_t i = 0; i < vertices.size(); i++) {
			this->vertices.push_back(vertices.at(i));
		}
	}


	/**
	 * Destrutor da classe TrianglesDrawingImpl (por defeito em c++).
	 */
	~TrianglesDrawingImpl(void) = default;
};


/**
 * Construtor por defeito. Inicializa o apontador para a implementação da
 * classe TrianglesDrawing.
 */
TrianglesDrawing::TrianglesDrawing(void)
{
	pimpl = new TrianglesDrawingImpl();
}


/**
 * Construtor por parâmetros. Inicializa o apontador para a implementação da
 * classe TrianglesDrawing através dos parâmetros recebidos.
 *
 * @param vertices Conjunto de vértices a desenhar.
 */
TrianglesDrawing::TrianglesDrawing(std::vector<Vertex> vertices)
{
	pimpl = new TrianglesDrawingImpl(vertices);
}


/**
 * Construtor de cópia. Inicializa o apontador para a implementação da classe
 * TrianglesDrawing, recorrendo a uma deep copy de um objeto desta classe.
 *
 * @param t Objeto da classe TrianglesDrawing a copiar.
 */
TrianglesDrawing::TrianglesDrawing(const TrianglesDrawing& t)
{
	pimpl = new TrianglesDrawingImpl(t.pimpl->getVertices());
}


/**
 * Devolve o conjunto de vértices a desenhar.
 */
std::vector<Vertex> TrianglesDrawing::getVertices(void)
{
	return pimpl->getVertices();
}


/**
 * Altera o conjunto de vértices a desenhar.
 */
void TrianglesDrawing::setVertices(std::vector<Vertex> vertices)
{
	pimpl->setVertices(vertices);
}


void TrianglesDrawing::execute(void)
{
	std::vector<Vertex> vertices = pimpl->getVertices();

	glBegin(GL_TRIANGLES);

	for (size_t i = 0; i < vertices.size(); i++) {
		glVertex3f(vertices.at(i).getX(),
			       vertices.at(i).getY(),
			       vertices.at(i).getZ());
	}

	glEnd();
}


/**
 * Destrutor da classe TrianglesDrawing. Liberta a memória ocupada pelo 
 * apontador para a implementação da classe TrianglesDrawing.
 */
TrianglesDrawing::~TrianglesDrawing(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}