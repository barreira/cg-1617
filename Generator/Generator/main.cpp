#include <iostream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;


#define BOX    "box"


#define CONE   "cone"


#define PLANE "plane"
#define PLANE_PARAMS 3


#define SPHERE "sphere"


#define MIN_PARAMS 5



bool isPlane(vector<string> commands, vector<float> &dim, string fileName)
{
	bool ret = true;

	if (commands.size() == PLANE_PARAMS) {
		fileName = commands.back();

		for (int i = 0; i < commands.size() - 1 && ret; i++) {
			istringstream iss(commands.at(i));
			float flt = 0;

			if (!(iss >> flt)) {
				ret = false;
			}
			else {
				dim.push_back(flt);
			}
		}
	}
	else {
		ret = false;
	}

	return ret;
}


vector<string> generateVertex(float x, float y, float z, float multX, float multY, float multZ)
{
	vector<string> ret;
	stringstream aux;

	aux << x << " " << y << " " << z;
	ret.push_back(aux.str());

	aux.str("");

	aux << -x << " " << -y << " " << -z;
	ret.push_back(aux.str());

	aux.str("");

	aux << (x * multX) << " " << (y * multY) << " " << (z * multZ);
	ret.push_back(aux.str());



	return ret;
}


bool generatePlane(vector<string> commands)
{
	vector<float> dim;
	string fileName;
	vector<string> v1;
	vector<string> v2;

	if (isPlane(commands, dim, fileName)) {
		float x = dim.front() / 2;
		float z = dim.back() / 2;

		v1 = generateVertex(x, 0, z, -1, 1, 1);
		v2 = generateVertex(x, 0, z, 1, 1, -1);


	}

	return true;
}



bool isValidCommands(string primitive, vector<string> commands)
{
	bool ret = false;
	/*
	if (primitive.compare(BOX) == 0) {
	ret = isBox(commands);
	}
	else if (primitive.compare(CONE) == 0) {
	ret = isCone(commands);
	}*/
	//else if (primitive.compare(PLANE) == 0) {
	ret = generatePlane(commands);
	/*}
	else if (primitive.compare(SPHERE) == 0) {
	ret = isSphere(commands);
	}*/

	return ret;
}


int main(int argc, char** argv)
{
	if (argc >= MIN_PARAMS) {
		string primitive(argv[1]);
		vector<string> commands;

		for (int i = 2; i < argc; i++) {
			commands.push_back(argv[i]);
		}

		if (isValidCommands(primitive, commands)) {

		}
	}

	return 0;
}