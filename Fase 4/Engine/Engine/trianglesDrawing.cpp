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
 * @version 02-05-2017
 */


#include "trianglesDrawing.h"


class TrianglesDrawing::TrianglesDrawingImpl {
	std::vector<GLfloat> vertices;  // Conjunto de vértices a desenhar
	std::vector<GLfloat> normals;   // Conjunto de normais a desenhar
	std::vector<size_t> indexes;    // Conjunto de índices associados ao vetor
	                                // de vértices
	Color* color;                   // Cor do modelo

	bool isGenerated;               // Flag que testa se o buffer já foi 
	                                // inicializado ou não

	GLuint buffers[2];              // Buffers utilizados para os VBOs


	/**
	 * Gera os buffers para serem utilizado em VBOs. 
	 * Preenche o buffer com o array de íncices.
	 */
	void generate(void)
	{
		// O buffer é apenas inicalizado uma vez
		isGenerated = true;

		glGenBuffers(2, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);
	}


public:

	/**
	 * Construtor por defeito.
	 */
	TrianglesDrawingImpl(void)
	{
		color = NULL;
	}


	/**
	 * Construtor por parâmetros.
	 *
	 * @param vertices Conjunto de vértices a desenhar.
	 * @param normals  Conjunto de vértices a desenhar.
	 * @param indexes  Conjunto de índices associados ao vetor de vértices.
	 * @param color    Cor do modelo.
	 */
	TrianglesDrawingImpl(std::vector<GLfloat> vertices, 
		                 std::vector<GLfloat> normals,
		                 std::vector<size_t> indexes,
		                 Color* color)
	{
		this->vertices = vertices;
		this->normals = normals;
		this->indexes = indexes;
		this->color = color;

		isGenerated = false;
	}


	/**
	 * Devolve o conjunto de vértices a desenhar.
	 */
	std::vector<GLfloat> getVertices(void)
	{
		return vertices;
	}


	/**
	 * Devolve o conjunto de normais a desenhar.
	 */
	std::vector<GLfloat> getNormals(void)
	{
		return normals;
	}


	/**
	 * Devolve o conjunto de índices associados ao vetor de vértices.
	 */
	std::vector<size_t> getIndexes(void)
	{
		return indexes;
	}


	/**
	 * Devolve a cor do modelo.
	 */
	Color* getColor(void)
	{
		return color;
	}


	/**
	 * Altera o conjunto de vértices a desenhar.
	 */
	void setVertices(std::vector<GLfloat> vertices)
	{
		this->vertices = vertices;
	}


	/**
	 * Altera o conjunto de normais a desenhar.
	 */
	void setNormals(std::vector<GLfloat> normals)
	{
		this->normals = normals;
	}


	/**
	 * Altera o conjunto de índices associados ao vetor de vértices.
	 */
	void setIndexes(std::vector<size_t> indexes)
	{
		this->indexes = indexes;
	}


	/** 
	 * Altera a cor do modelo.
	 */
	void setColor(Color* color)
	{
		this->color = color;
	}


	/**
	 * Desenha uma primitiva em modo de VBOs com um conjunto de índices.
	 */
	void execute(void)
	{
		if (isGenerated == false) {
			generate();
		}

		color->colorize();

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, &indexes[0]);
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
 * @param normals  Conjunto de normais a desenhar.
 * @param indexes  Conjunto de índices associados ao vetor de vértices.
 * @param color    Cor do modelo.
 */
TrianglesDrawing::TrianglesDrawing(std::vector<GLfloat> vertices,
                                   std::vector<GLfloat> normals,
                                   std::vector<size_t> indexes,
	                               Color* color)
{
	pimpl = new TrianglesDrawingImpl(vertices, normals, indexes,
		                             color);
}


/**
 * Construtor de cópia. Inicializa o apontador para a implementação da classe
 * TrianglesDrawing, recorrendo a uma deep copy de um objeto desta classe.
 *
 * @param t Objeto da classe TrianglesDrawing a copiar.
 */
TrianglesDrawing::TrianglesDrawing(const TrianglesDrawing& t)
{
	pimpl = new TrianglesDrawingImpl(t.pimpl->getVertices(),
		                             t.pimpl->getNormals(),
		                             t.pimpl->getIndexes(),
		                             t.pimpl->getColor());
}


/**
 * Devolve o conjunto de vértices a desenhar.
 */
std::vector<GLfloat> TrianglesDrawing::getVertices(void)
{
	return pimpl->getVertices();
}


/**
 * Devolve o conjunto de normais a desenhar.
 */
std::vector<GLfloat> TrianglesDrawing::getNormals(void)
{
	return pimpl->getNormals();
}


/**
 * Devolve o conjunto de índices associados ao vetor de vértices.
 */
std::vector<size_t> TrianglesDrawing::getIndexes(void)
{
	return pimpl->getIndexes();
}


/**
 * Devolve a cor do modelo.
 */
Color* TrianglesDrawing::getColor(void)
{
	return pimpl->getColor();
}


/**
 * Altera o conjunto de vértices a desenhar.
 */
void TrianglesDrawing::setVertices(std::vector<GLfloat> vertices)
{
	pimpl->setVertices(vertices);
}


/**
 * Altera o conjunto de normais a desenhar.
 */
void TrianglesDrawing::setNormals(std::vector<GLfloat> normals)
{
	pimpl->setNormals(normals);
}


/**
 * Altera o conjunto de índices associados ao vetor de vértices.
 */
void TrianglesDrawing::setIndexes(std::vector<size_t> indexes)
{
	pimpl->setIndexes(indexes);
}


/**
 * Altera a cor do modelo.
 */
void TrianglesDrawing::setColor(Color* color)
{
	pimpl->setColor(color);
}


/**
 * Desenha os vértices correspondentes a uma primitiva em OpenGL.
 */
void TrianglesDrawing::execute(void)
{
	pimpl->execute();
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