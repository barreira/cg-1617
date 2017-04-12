/**
 * @file translation.cpp
 *
 * Definição de todos os métodos para se representar uma translação em OpenGL.
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 12-04-2017
 */


#include <cstddef>
#include "translation.h"


class Translation::TranslationImpl {
	float x;                            // Valor de uma translação em x
	float y;                            // Valor de uma translação em y
	float z;                            // Valor de uma translação em z
	std::vector<Vertex> catmullPoints;  // Lista de pontos para uma curva do
	                                    // tipo Catmull-Rom
	size_t elapsedTime;                 // Tempo decorrido desde o ínicio da
	                                    // contagem de tempo
	float totalTime;                    // Tempo total da animação
	float timeAcc;                      // Tempo decorrido da animação
	float up[3];                        // Normal de um modelo


	/**
	 * Devolve o intervalo de tempo decorrido deste a última medida de tempo
	 * efetuada.
	 */
	size_t getDeltaTime(void)
	{
		size_t actualTime = glutGet(GLUT_ELAPSED_TIME);
		size_t deltaTime = actualTime - elapsedTime;

		elapsedTime = actualTime;

		return deltaTime;
	}


	void buildRotMatrix(float* x, float* y, float* z, float* m)
	{
		m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
		m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
		m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
	}


	void cross(float* a, float* b, float* res) 
	{
		res[0] = a[1] * b[2] - a[2] * b[1];
		res[1] = a[2] * b[0] - a[0] * b[2];
		res[2] = a[0] * b[1] - a[1] * b[0];
	}


	void normalize(float* a) 
	{
		float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
		
		a[0] = a[0] / l;
		a[1] = a[1] / l;
		a[2] = a[2] / l;
	}


	float length(float* v) 
	{
		float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		
		return res;
	}


	void multMatrixVector(float* m, float* v, float* res) 
	{
		for (int j = 0; j < 4; ++j) {
			res[j] = 0;
			
			for (int k = 0; k < 4; ++k) {
				res[j] += v[k] * m[j * 4 + k];
			}
		}
	}


	void getCatmullRomPoint(float t, 
		                    Vertex v0, Vertex v1, Vertex v2, Vertex v3, 
		                    float* pos, float* deriv) 
	{

		// catmull-rom matrix
		float m[4][4] = { { -0.5f,  1.5f, -1.5f,  0.5f },
		                  { 1.0f, -2.5f,  2.0f, -0.5f },
		                  { -0.5f,  0.0f,  0.5f,  0.0f },
		                  { 0.0f,  1.0f,  0.0f,  0.0f } };

		// Compute A = M * P

		float ax[4];
		float ay[4];
		float az[4];

		float px[4] = { v0.getX(), v1.getX(), v2.getX(), v3.getX() };
		float py[4] = { v0.getY(), v1.getY(), v2.getY(), v3.getY() };
		float pz[4] = { v0.getZ(), v1.getZ(), v2.getZ(), v3.getZ() };

		multMatrixVector((float*)m, px, ax);
		multMatrixVector((float*)m, py, ay);
		multMatrixVector((float*)m, pz, az);

		// Compute pos = T * A

		float T[4] = { t * t * t, t * t, t, 1 };

		pos[0] = (T[0] * ax[0]) + (T[1] * ax[1]) + (T[2] * ax[2]) + (T[3] * ax[3]);
		pos[1] = (T[0] * ay[0]) + (T[1] * ay[1]) + (T[2] * ay[2]) + (T[3] * ay[3]);
		pos[2] = (T[0] * az[0]) + (T[1] * az[1]) + (T[2] * az[2]) + (T[3] * az[3]);

		// compute deriv = T' * A

		float TDeriv[4] = { 3 * (t * t), 2 * t, 1, 0 };

		deriv[0] = (TDeriv[0] * ax[0]) + (TDeriv[1] * ax[1]) + (TDeriv[2] * ax[2]) + (TDeriv[3] * ax[3]);
		deriv[1] = (TDeriv[0] * ay[0]) + (TDeriv[1] * ay[1]) + (TDeriv[2] * ay[2]) + (TDeriv[3] * ay[3]);
		deriv[2] = (TDeriv[0] * az[0]) + (TDeriv[1] * az[1]) + (TDeriv[2] * az[2]) + (TDeriv[3] * az[3]);
	}


	// given  global t, returns the point in the curve3
	void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv) 
	{
		size_t numPoints = catmullPoints.size();
		int indexes[4]; // indices store the points

		float t = gt * numPoints; // this is the real global t
		int index = floor(t);  // which segment
		
		t = t - index; // where within  the segment

		indexes[0] = (index + numPoints - 1) % numPoints;
		indexes[1] = (indexes[0] + 1) % numPoints;
		indexes[2] = (indexes[1] + 1) % numPoints;
		indexes[3] = (indexes[2] + 1) % numPoints;

		getCatmullRomPoint(t, 
			               catmullPoints.at(indexes[0]), 
			               catmullPoints.at(indexes[1]), 
			               catmullPoints.at(indexes[2]),
			               catmullPoints.at(indexes[3]),
			               pos, deriv);
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
	 * Construtor por parâmetros.
	 *
	 * @param x Valor de uma translação em x.
	 * @param y Valor de uma translação em y.
 	 * @param z Valor de uma translação em z.
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
	* Construtor por parâmetros.
	*
	* @param x Valor de uma translação em x.
	* @param y Valor de uma translação em y.
	* @param z Valor de uma translação em z.
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
	 * Devolve o valor de uma translação em x.
	 */
	float getX(void)
	{
		return x;
	}


	/**
	 * Devolve o valor de uma translação em y.
	 */
	float getY(void)
	{
		return y;
	}


	/**
	 * Devolve o valor de uma translação em z.
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
	 * Altera o valor de uma translação em x.
	 */
	void setX(float x)
	{
		this->x = x;
	}


	/**
	 * Altera o valor de uma translação em x.
	 */
	void setY(float y)
	{
		this->y = y;
	}


	/**
	 * Altera o valor de uma translação em x.
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
 * Construtor por defeito. Inicializa o apontador para a implementação da
 * classe Translation.
 */
Translation::Translation(void)
{
	pimpl = new TranslationImpl();
}


/**
 * Construtor por parâmetros. Inicializa o apontador para a implementação da
 * classe Translation através dos parâmetros recebidos.
 *
 * @param x Valor de uma translação em x.
 * @param y Valor de uma translação em y.
 * @param z Valor de uma translação em z.
 */
Translation::Translation(float x, float y, float z)
{
	pimpl = new TranslationImpl(x, y, z);
}


/**
 * Construtor por parâmetros. Inicializa o apontador para a implementação da
 * classe Translation através dos parâmetros recebidos.
 *
 * @param x Valor de uma translação em x.
 * @param y Valor de uma translação em y.
 * @param z Valor de uma translação em z.
 */
Translation::Translation(float x, float y, float z, 
	                     std::vector<Vertex> catmullPoints, float totalTime)
{
	pimpl = new TranslationImpl(x, y, z, catmullPoints, totalTime);
}


/**
 * Construtor de cópia. Inicializa o apontador para a implementação da classe
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
 * Devolve o valor de uma translação em x.
 */
float Translation::getX(void)
{
	return pimpl->getX();
}


/**
 * Devolve o valor de uma translação em y.
 */
float Translation::getY(void)
{
	return pimpl->getY();
}


/**
 * Devolve o valor de uma translação em z.
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
 * Altera o valor de uma translação em x.
 */
void Translation::setX(float x)
{
	pimpl->setX(x);
}


/**
 * Altera o valor de uma translação em y.
 */
void Translation::setY(float y)
{
	pimpl->setY(y);
}


/**
 * Altera o valor de uma translação em z.
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
 * Executa uma translação em x, y ou z em OpenGL.
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
 * Destrutor da classe Translation. Liberta a memória ocupada pelo apontador 
 * para a implementação da classe Translation.
 */
Translation::~Translation(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}