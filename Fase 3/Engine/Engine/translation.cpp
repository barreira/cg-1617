/**
 * @file translation.cpp
 *
 * Defini��o de todos os m�todos para se representar uma transla��o em OpenGL.
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 21-04-2017
 */


#include <cstddef>
#include "translation.h"
#include "cubicCurves.h"


class Translation::TranslationImpl {
	float x;                            // Valor de uma transla��o em x
	float y;                            // Valor de uma transla��o em y
	float z;                            // Valor de uma transla��o em z
	std::vector<Vertex> catmullPoints;  // Lista de pontos para uma curva do
	                                    // tipo Catmull-Rom
	size_t elapsedTime;                 // Tempo decorrido desde o �nicio da
	                                    // contagem de tempo
	float totalTime;                    // Tempo total da anima��o
	float timeAcc;                      // Tempo decorrido da anima��o
	float up[3];                        // Normal de um modelo


	/**
	 * Devolve o intervalo de tempo decorrido deste a �ltima medida de tempo
	 * efetuada.
	 */
	size_t getDeltaTime(void)
	{
		size_t actualTime = glutGet(GLUT_ELAPSED_TIME);
		size_t deltaTime = actualTime - elapsedTime;

		elapsedTime = actualTime;

		return deltaTime;
	}


	// given  global t, returns the point in the curve3
	void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) 
	{
		size_t numPoints = catmullPoints.size();
		int indexes[4]; // indices store the points

		float t = gt * numPoints; // this is the real global t
		int index = floor(t);  // which segment
		
		float p[12];

		t = t - index; // where within  the segment

		indexes[0] = (index + numPoints - 1) % numPoints;
		indexes[1] = (indexes[0] + 1) % numPoints;
		indexes[2] = (indexes[1] + 1) % numPoints;
		indexes[3] = (indexes[2] + 1) % numPoints;
		 
		for (size_t i = 0; i < 4; i++) {
			p[i * 3] = catmullPoints.at(indexes[i]).getX();
			p[i * 3 + 1] = catmullPoints.at(indexes[i]).getY();
			p[i * 3 + 2] = catmullPoints.at(indexes[i]).getZ();
		}

		getCatmullRomPoint(t, p, p + 3, p + 6, p + 9, pos, deriv);
	}


public:

	/**
	 * Construtor por defeito.
	 */
	TranslationImpl(void)
	{
		x = y = z = 0.0;
		totalTime = timeAcc = 0.0;
		elapsedTime = 0;
		up[0] = 0.0;
		up[1] = 1.0;
		up[2] = 0.0;
	}


	/**
	 * Construtor por par�metros.
	 *
	 * @param x Valor de uma transla��o em x.
	 * @param y Valor de uma transla��o em y.
 	 * @param z Valor de uma transla��o em z.
	 */
	TranslationImpl(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;

		totalTime = timeAcc = 0.0;
		elapsedTime = 0;

		up[0] = 0.0;
		up[1] = 1.0;
		up[2] = 0.0;
	}


	/**
	* Construtor por par�metros.
	*
	* @param x Valor de uma transla��o em x.
	* @param y Valor de uma transla��o em y.
	* @param z Valor de uma transla��o em z.
	*/
	TranslationImpl(float x, float y, float z,
		            std::vector<Vertex> catmullPoints, float totalTime)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->catmullPoints = catmullPoints;
		this->totalTime = totalTime;

		elapsedTime = 0;
		timeAcc = 0.0;

		up[0] = 0.0;
		up[1] = 1.0;
		up[2] = 0.0;
	}


	/**
	 * Devolve o valor de uma transla��o em x.
	 */
	float getX(void)
	{
		return x;
	}


	/**
	 * Devolve o valor de uma transla��o em y.
	 */
	float getY(void)
	{
		return y;
	}


	/**
	 * Devolve o valor de uma transla��o em z.
	 */
	float getZ(void)
	{
		return z;
	}


	std::vector<Vertex> getCatmullPoints(void)
	{
		return catmullPoints;
	}


	float getTotalTime(void)
	{
		return totalTime;
	}


	/**
	 * Altera o valor de uma transla��o em x.
	 */
	void setX(float x)
	{
		this->x = x;
	}


	/**
	 * Altera o valor de uma transla��o em x.
	 */
	void setY(float y)
	{
		this->y = y;
	}


	/**
	 * Altera o valor de uma transla��o em x.
	 */
	void setZ(float z)
	{
		this->z = z;
	}


	void setCatmullPoints(std::vector<Vertex> catmullPoints)
	{
		this->catmullPoints = catmullPoints;
		timeAcc = 0.0;
		elapsedTime = 0;
	}


	void setTotalTime(float totalTime)
	{
		this->totalTime = totalTime;
		timeAcc = 0.0;
		elapsedTime = 0;
	}


	void execute(void)
	{
		float pos[3];
		float deriv[3];
		float z[3];

		size_t deltaTime = getDeltaTime();
		float auxTime = ((float)deltaTime) / 1000.0;

		timeAcc += (auxTime / (float)totalTime);

		if (timeAcc >= 1.0) {
			timeAcc = 0.0;
		}

		getGlobalCatmullRomPoint(timeAcc, pos, deriv);
		glTranslatef(pos[0], pos[1], pos[2]);
		
		cross(deriv, up, z);
		normalize(z);

		cross(z, deriv, up);
		normalize(up);

		normalize(deriv);

		float m[4][4] = {
			{ deriv[0], up[0], z[0], pos[0] },
			{ deriv[1], up[1], z[1], pos[1] },
			{ deriv[2], up[2], z[2], pos[2] },
			{ 0.0, 0.0, 0.0, 1.0 }
		};

		buildRotMatrix(deriv, up, z, (float*)m);
		glMultMatrixf((float*)m);
	}


	/**
	 * Destrutor da classe TranslationImpl (por defeito em c++).
	 */
	~TranslationImpl(void) = default;
};


/**
 * Construtor por defeito. Inicializa o apontador para a implementa��o da
 * classe Translation.
 */
Translation::Translation(void)
{
	pimpl = new TranslationImpl();
}


/**
 * Construtor por par�metros. Inicializa o apontador para a implementa��o da
 * classe Translation atrav�s dos par�metros recebidos.
 *
 * @param x Valor de uma transla��o em x.
 * @param y Valor de uma transla��o em y.
 * @param z Valor de uma transla��o em z.
 */
Translation::Translation(float x, float y, float z)
{
	pimpl = new TranslationImpl(x, y, z);
}


/**
 * Construtor por par�metros. Inicializa o apontador para a implementa��o da
 * classe Translation atrav�s dos par�metros recebidos.
 *
 * @param x Valor de uma transla��o em x.
 * @param y Valor de uma transla��o em y.
 * @param z Valor de uma transla��o em z.
 */
Translation::Translation(float x, float y, float z, 
	                     std::vector<Vertex> catmullPoints, float totalTime)
{
	pimpl = new TranslationImpl(x, y, z, catmullPoints, totalTime);
}


/**
 * Construtor de c�pia. Inicializa o apontador para a implementa��o da classe
 * Translation, recorrendo a uma deep copy de um objeto desta classe.
 *
 * @param t Objeto da classe Translation a copiar.
 */
Translation::Translation(const Translation& t)
{
	pimpl = new TranslationImpl(t.pimpl->getX(),
		                        t.pimpl->getY(), t.pimpl->getZ(),
		                        t.pimpl->getCatmullPoints(),
		                        t.pimpl->getTotalTime());
}


/**
 * Devolve o valor de uma transla��o em x.
 */
float Translation::getX(void)
{
	return pimpl->getX();
}


/**
 * Devolve o valor de uma transla��o em y.
 */
float Translation::getY(void)
{
	return pimpl->getY();
}


/**
 * Devolve o valor de uma transla��o em z.
 */
float Translation::getZ(void)
{
	return pimpl->getZ();
}


std::vector<Vertex> Translation::getCatmullPoints(void)
{
	return pimpl->getCatmullPoints();
}


float Translation::getTotalTime(void)
{
	return pimpl->getTotalTime();
}


/**
 * Altera o valor de uma transla��o em x.
 */
void Translation::setX(float x)
{
	pimpl->setX(x);
}


/**
 * Altera o valor de uma transla��o em y.
 */
void Translation::setY(float y)
{
	pimpl->setY(y);
}


/**
 * Altera o valor de uma transla��o em z.
 */
void Translation::setZ(float z)
{
	pimpl->setZ(z);
}


void Translation::setCatmullPoints(std::vector<Vertex> catmullPoints)
{
	pimpl->setCatmullPoints(catmullPoints);
}


void Translation::setTotalTime(float totalTime)
{
	pimpl->setTotalTime(totalTime);
}


/**
 * Executa uma transla��o em x, y ou z em OpenGL.
 */
void Translation::execute(void)
{
	if (pimpl->getCatmullPoints().size() < 4) {
		glTranslatef(pimpl->getX(), pimpl->getY(), pimpl->getZ());
	}
	else {
		pimpl->execute();
	}
}


/**
 * Destrutor da classe Translation. Liberta a mem�ria ocupada pelo apontador 
 * para a implementa��o da classe Translation.
 */
Translation::~Translation(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}