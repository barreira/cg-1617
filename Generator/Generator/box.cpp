#include <algorithm>
#include "box.h"


class Box::BoxImpl {
	float dimX;
	float dimY;
	float dimZ;
	float divX;
	float divY;
	float divZ;
	float centerX;
	float centerY;
	float centerZ;
	size_t numDiv;


public:
	BoxImpl(void)
	{
		dimX = dimY = dimZ = divX = divY = divZ = centerX = centerY = centerZ = 0;
		numDiv = 0;
	}


	BoxImpl(float dimX, float dimY, float dimZ, size_t numDiv)
	{
		this->dimX = dimX;
		this->dimY = dimY;
		this->dimZ = dimZ;
		this->numDiv = numDiv;

		divX = dimX / ((float) numDiv + 1);
		divY = dimY / ((float) numDiv + 1);
		divZ = dimZ / ((float) numDiv + 1);

		centerX = dimX / 2;
		centerY = dimY / 2;
		centerZ = dimZ / 2;
	}


	float getDimX(void)
	{
		return dimX;
	}


	float getDimY(void)
	{
		return dimY;
	}


	float getDimZ(void)
	{
		return dimZ;
	}


	size_t getNumDiv(void)
	{
		return numDiv;
	}


	std::vector<Vertex*> genereteFaceXY(float x, float y, float z)
	{
		std::vector<Vertex*> vertices;

		for (float i = x; i < dimX; i += divX) {
			for (float j = y; j < dimY; j += divY) {
				vertices.push_back(new Vertex(i - centerX, j - centerY, z - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, j - centerY, z - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, j + divY - centerY, z - centerZ));

				vertices.push_back(new Vertex(i - centerX, j - centerY, z - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, j + divY - centerY, z - centerZ));
				vertices.push_back(new Vertex(i - centerX, j + divY - centerY, z - centerZ));

				vertices.push_back(new Vertex(i - centerX, j - centerY, z - dimZ - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, j + divY - centerY, z - dimZ - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, j - centerY, z - dimZ - centerZ));

				vertices.push_back(new Vertex(i - centerX, j - centerY, z - dimZ - centerZ));
				vertices.push_back(new Vertex(i - centerX, j + divY - centerY, z - dimZ - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, j + divY - centerY, z - dimZ - centerZ));
			}
		}

		return vertices;
	}


	std::vector<Vertex*> genereteFaceXZ(float x, float y, float z)
	{
		std::vector<Vertex*> vertices;

		for (float i = x; i < dimX; i += divX) {
			for (float j = z; j < dimZ; j += divZ) {
				vertices.push_back(new Vertex(i - centerX, y - centerY, j - centerZ));
				vertices.push_back(new Vertex(i - centerX, y - centerY, j + divZ - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, y - centerY, j - centerZ));

				vertices.push_back(new Vertex(i + divX - centerX, y - centerY, j - centerZ));
				vertices.push_back(new Vertex(i - centerX, y - centerY, j + divZ - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, y - centerY, j + divZ - centerZ));


				vertices.push_back(new Vertex(i - centerX, y - centerY - dimY, j - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, y - centerY - dimY, j - centerZ));
				vertices.push_back(new Vertex(i - centerX, y - centerY - dimY, j + divZ - centerZ));
				
				vertices.push_back(new Vertex(i + divX - centerX, y - centerY - dimY, j - centerZ));
				vertices.push_back(new Vertex(i + divX - centerX, y - centerY - dimY, j + divZ - centerZ));
				vertices.push_back(new Vertex(i - centerX, y - centerY - dimY, j + divZ - centerZ));
			}
		}

		return vertices;
	}


	std::vector<Vertex*> genereteFaceYZ(float x, float y, float z)
	{
		std::vector<Vertex*> vertices;

		for (float i = y; i < dimY; i += divY) {
			for (float j = z; j < dimZ; j += divZ) {
				vertices.push_back(new Vertex(x - centerX, i - centerY, j - centerZ));
				vertices.push_back(new Vertex(x - centerX, i + divY - centerY, j - centerZ));
				vertices.push_back(new Vertex(x - centerX, i + divY - centerY, j + divZ - centerZ));

				vertices.push_back(new Vertex(x - centerX, i - centerY, j - centerZ));
				vertices.push_back(new Vertex(x - centerX, i + divY - centerY, j + divZ - centerZ));
				vertices.push_back(new Vertex(x - centerX, i - centerY, j + divZ - centerZ));


				vertices.push_back(new Vertex(x - centerX - dimX, i - centerY, j - centerZ));
				vertices.push_back(new Vertex(x - centerX - dimX, i + divY - centerY, j + divZ - centerZ));
				vertices.push_back(new Vertex(x - centerX - dimX, i + divY - centerY, j - centerZ));

				vertices.push_back(new Vertex(x - centerX - dimX, i - centerY, j - centerZ));
				vertices.push_back(new Vertex(x - centerX - dimX, i - centerY, j + divZ - centerZ));
				vertices.push_back(new Vertex(x - centerX - dimX, i + divY - centerY, j + divZ - centerZ));
			}
		}

		return vertices;
	}


	~BoxImpl(void) = default;
};


Box::Box(void) : pimpl{ new BoxImpl() } {}


Box::Box(float dimX, float dimY, float dimZ, size_t numDiv)
{
	numDiv = (numDiv < 0) ? 0 : numDiv;

	pimpl = std::unique_ptr<BoxImpl>(new BoxImpl(dimX, dimY, dimZ, numDiv));
}


float Box::getDimX(void)
{
	return pimpl->getDimX();
}


float Box::getDimY(void)
{
	return pimpl->getDimY();
}


float Box::getDimZ(void)
{
	return pimpl->getDimZ();
}


size_t Box::getNumDiv(void)
{
	return pimpl->getNumDiv();
}


void Box::generateVertices(void)
{
	std::vector<Vertex*> verticesXY = pimpl->genereteFaceXY(0, 0, pimpl->getDimZ());
	std::vector<Vertex*> verticesXZ = pimpl->genereteFaceXZ(0, pimpl->getDimY(), 0);
	std::vector<Vertex*> verticesYZ = pimpl->genereteFaceYZ(pimpl->getDimX(), 0, 0);

	for (size_t i = 0; i < verticesXY.size(); i++) {
		addVertex(verticesXY.at(i));
	}

	for (size_t i = 0; i < verticesXZ.size(); i++) {
		addVertex(verticesXZ.at(i));
	}

	for (size_t i = 0; i < verticesYZ.size(); i++) {
		addVertex(verticesYZ.at(i));
	}
}


Box::~Box(void) = default;