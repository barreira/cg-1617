#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <math.h>
#include "plane.h"
#include "box.h"
#include "cone.h"
#include "sphere.h"


#define BOX    "box"
#define BOX_ARGS 4

#define CONE   "cone"
#define CONE_ARGS 5
#define CONE_CEILS 2

#define PLANE "plane"
#define PLANE_ARGS 3
#define PLANE_CEILS 0

#define SPHERE "sphere"
#define SPHERE_ARGS 4
#define SPHERE_CEILS 2

#define MIN_PARAMS 5


std::vector<Vertex> vertices;
std::string fileName;
std::vector<float> arguments;
Primitive *p;


bool generateArgs(std::vector<std::string> params, const size_t numParams,
	              const size_t ceilParams)
{
	bool ret = true;

	if (params.size() == numParams) {
		fileName = params.back();

		for (size_t i = 0; i < params.size() - 1 && ret; i++) {
			std::stringstream ss(params.at(i));
			
			float flt = 0;

			if (!(ss >> flt)) {
				ret = false;
			}
			else {
				if (i < params.size() - 1 - ceilParams) {
					arguments.push_back(flt);
				}
				else {
					arguments.push_back(ceil(flt));
				}
			}
		}
	}
	else {
		ret = false;
	}

	return ret;
}


bool generateBox(std::vector<std::string> params)
{
	bool ret = false;
	size_t boxCeils = params.size() == BOX_ARGS ? 0 : 1;

	ret = generateArgs(params, params.size(), boxCeils);

	if (ret) {
		if (boxCeils == 0) {
			p = new Box(arguments.at(0), arguments.at(1), arguments.at(2));
		}
		else {
			p = new Box(arguments.at(0), arguments.at(1), arguments.at(2),
				        (size_t) arguments.at(3));
		}


		p->generateVertices();
		vertices = p->getVertices();
	}

	return ret;
}


bool generateCone(std::vector<std::string> params)
{
	bool ret = generateArgs(params, CONE_ARGS, CONE_CEILS);

	if (ret) {
		p = new Cone(arguments.at(0), arguments.at(1),
			         (size_t) arguments.at(2), (size_t) arguments.at(3));
		p->generateVertices();
		vertices = p->getVertices();
	}

	return ret;
}


bool generatePlane(std::vector<std::string> params)
{
	bool ret = generateArgs(params, PLANE_ARGS, PLANE_CEILS);

	if (ret) {
		p = new Plane(arguments.at(0), arguments.at(1));
		p->generateVertices();
		vertices = p->getVertices();
	}

	return ret;
}


bool generateSphere(std::vector<std::string> params)
{
	bool ret = generateArgs(params, SPHERE_ARGS, SPHERE_CEILS);

	if (ret) {
		p = new Sphere(arguments.at(0),
			           (size_t)arguments.at(1), (size_t)arguments.at(2));
		p->generateVertices();
		vertices = p->getVertices();
	}

	return ret;
}


bool generatePrimitive(std::string primitive, std::vector<std::string> params)
{
	bool ret = false;
	
	if (primitive.compare(BOX) == 0) {
		ret = generateBox(params);
	}
	else if (primitive.compare(CONE) == 0) {
		ret = generateCone(params);
	}
	else if (primitive.compare(PLANE) == 0) {
		ret = generatePlane(params);
	}
	else if (primitive.compare(SPHERE) == 0) {
		ret = generateSphere(params);
	}

	return ret;
}


int main(int argc, char** argv)
{
	bool ok = false;
	std::ofstream file;

	if (argc >= MIN_PARAMS) {
		std::string primitive(argv[1]);
		std::vector<std::string> params;

		for (int i = 2; i < argc; i++) {
			params.push_back(argv[i]);
		}

		ok = generatePrimitive(primitive, params);
	}

	if (ok) {
		file.open(fileName);

		for (size_t i = 0; i < vertices.size(); i++) {
			file << vertices.at(i).toString() << std::endl;
		}
	}
	else {
		std::cout << "Invalid Arguments!" << std::endl;
	}

	getchar();
	return 0;
}