#include <stack>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "xmlParser.h"
#include "tinyxml.h"
#include "popMatrix.h"
#include "pushMatrix.h"
#include "rotation.h"
#include "scale.h"
#include "translation.h"
#include "trianglesDrawing.h"


#define SCENE  "scene"
#define GROUP  "group"
#define MODELS "models"

/* Tag de model e atributos */
#define MODEL "model"
#define FILE  "file"

/* Tag de rotate e atributos */
#define ROTATE "rotate"
#define ANGLE  "angle"
#define AXISX  "axisx"
#define AXISY  "axisy"
#define AXISZ  "axisz"

/* Tags de scale e translate e respetivos atributos */
#define SCALE     "scale"
#define TRANSLATE "translate"
#define X         "x"
#define Y         "y"
#define Z         "z"


class XMLParser::XMLParserImpl {
	TiXmlDocument doc;
	bool invalidDoc;
	size_t numModels;
	size_t failedModels;

	std::stack<bool> modelsInContainer;
	std::stack<size_t> numModelsInContainer;

	std::vector<GLOperation*> glOperations;
	std::string errorString;
	std::string fileName;


	void clearGLOperations(void)
	{
		for (size_t i = 0; i < glOperations.size(); i++) {
			delete glOperations.at(i);
		}

		glOperations.clear();
	}


	std::string toLower(const char* str)
	{
		std::string aux(str);

		std::transform(aux.begin(), aux.end(), aux.begin(), ::tolower);

		return aux;
	}


	void addModelsInContainer(void)
	{
		size_t aux = numModelsInContainer.top();

		numModelsInContainer.pop();
		numModelsInContainer.push(++aux);
		modelsInContainer.pop();
		modelsInContainer.push(true);
	}



	/**
	* Abre o ficheiro que contém um conjunto de vértices de uma primitiva.
	* Este ficheiro é aberto apenas uma vez.
	*
	* @param fileName Nome do ficheiro.
	*/
	std::vector<Vertex> readVertices(const char* file)
	{
		std::fstream fp;       // Estrutura para abertura de um ficheiro em modo
							   // de leitura
		std::string line;      // String auxiliar que irá corresponder a uma linha
							   // do ficheiro
		std::stringstream ss;  // String auxiliar para converter texto em valores
							   // numéricos

		std::vector<Vertex> vertices;

		float x = 0;  // Coordenada x de um vértice
		float y = 0;  // Coordenada y de um vértice
		float z = 0;  // Coordenada z de um vértice

		fp.open(file);

		// Testa se o ficheiro foi bem aberto e inicia a sua leitura
		if (fp.is_open()) {

			// Lê linha a linha
			while (getline(fp, line)) {
				std::stringstream ss(line);

				// Converte uma linha para as coordenadas x, y e z
				ss >> x >> y >> z;

				// Adiciona as coordenadas ao vetor de vértices
				vertices.push_back(Vertex(x, y, z));
			}

			fp.close();
		}
		else {
			errorString.append("Warning: Could not read file ");
			errorString.append(file);
			errorString.append("!\n");

			// Se não se conseguiu abrir bem o ficheiro então o módulo atual conta
			// como um módulo que teve um problema
			failedModels++;
		}

		return vertices;
	}


	void parseModel(TiXmlElement* model)
	{
		std::vector<Vertex> vertices;
		size_t numFiles = 0;

		numModels++;

		for (TiXmlAttribute* a = model->FirstAttribute(); 
		     a != NULL && invalidDoc == false; a = a->Next()) {
			
			std::string attName(toLower(a->Name()));

			if (attName.compare(FILE) == 0) {
				if (numFiles > 0) {
					invalidDoc = true;
				}
				else {
					vertices = readVertices(a->Value());
					numFiles++;
				}
			}
			else {
				errorString.append("Error: Invalid model attribute!\n");
				invalidDoc = true;
			}
		}

		glOperations.push_back(new TrianglesDrawing(vertices));
	}


	void parseModels(TiXmlElement* models)
	{
		for (TiXmlElement* model = models->FirstChildElement(); 
		     model != NULL && invalidDoc == false; 
		     model = model->NextSiblingElement()) {
			
			std::string modelName(toLower(model->Value()));

			if (modelName.compare(MODEL) == 0) {
				parseModel(model);
			}
			else {
				errorString.append("Error: Invalid models tag!\n");
				invalidDoc = true;
			}
		}
	}


	void parseGroup(TiXmlElement* group)
	{
		glOperations.push_back(new PushMatrix());

		for (TiXmlElement* tag = group->FirstChildElement(); 
		     tag != NULL && invalidDoc == false; 
		     tag = tag->NextSiblingElement()) {
			
			parseTag(tag);
		}

		numModelsInContainer.pop();
		modelsInContainer.pop();

		glOperations.push_back(new PopMatrix());
	}


	void parseTranslate(TiXmlElement* translate)
	{
		float x = 0;
		float y = 0;
		float z = 0;

		for (TiXmlAttribute* a = translate->FirstAttribute(); 
		     a != NULL && invalidDoc == false; a = a->Next()) {
			
			std::string attName(toLower(a->Name()));
			std::stringstream ss(a->Value());

			if (attName.compare(X) == 0) {
				ss >> x;
			}
			else if (attName.compare(Y) == 0) {
				ss >> y;
			}
			else if (attName.compare(Z) == 0) {
				ss >> z;
			}
			else {
				errorString.append("Error: Invalid translate attribute!\n");
				invalidDoc = true;
			}
		}

		glOperations.push_back(new Translation(x, y, z));
	}


	void parseRotate(TiXmlElement* rotate)
	{
		float angle = 0;
		float axisX = 0;
		float axisY = 0;
		float axisZ = 0;

		for (TiXmlAttribute* a = rotate->FirstAttribute(); 
		     a != NULL && invalidDoc == false; a = a->Next()) {
			
			std::string attName(toLower(a->Name()));
			std::stringstream ss(a->Value());

			if (attName.compare(ANGLE) == 0) {
				ss >> angle;
			}
			else if (attName.compare(AXISX) == 0) {
				ss >> axisX;
			}
			else if (attName.compare(AXISY) == 0) {
				ss >> axisY;
			}
			else if (attName.compare(AXISZ) == 0) {
				ss >> axisZ;
			}
			else {
				errorString.append("Error: Invalid rotate attribute!\n");
				invalidDoc = true;
			}
		}

		glOperations.push_back(new Rotation(angle, axisX, axisY, axisZ));
	}


	void parseScale(TiXmlElement* scale)
	{
		float x = 0;
		float y = 0;
		float z = 0;

		for (TiXmlAttribute* a = scale->FirstAttribute(); 
		     a != NULL && invalidDoc == false; a = a->Next()) {
			
			std::string attName(toLower(a->Name()));
			std::stringstream ss(a->Value());

			if (attName.compare(X) == 0) {
				ss >> x;
			}
			else if (attName.compare(Y) == 0) {
				ss >> y;
			}
			else if (attName.compare(Z) == 0) {
				ss >> z;
			}
			else {
				errorString.append("Error: Invalid scale attribute!\n");
				invalidDoc = true;
			}
		}

		glOperations.push_back(new Scale(x, y, z));
	}


	void parseTag(TiXmlElement* tag)
	{
		std::string tagName = toLower(tag->Value());

		// Testa se existe uma tag chamada model
		if (tagName.compare(MODELS) == 0) {
			if (numModelsInContainer.top() >= 1) {
				errorString.append("Error: Duplicated models tag in group!\n");
				invalidDoc = true;
			}
			else {
				addModelsInContainer();
				parseModels(tag);
			}
		}
		else if (tagName.compare(MODEL) == 0) {
			parseModel(tag);
		}
		else if (tagName.compare(GROUP) == 0) {
			modelsInContainer.push(false);
			numModelsInContainer.push(0);
			parseGroup(tag);
		}
		else if (tagName.compare(TRANSLATE) == 0) {
			if (modelsInContainer.top() == true) {
				errorString.append("Error: Misplaced translate tag!\n");
				invalidDoc = true;
			}
			else {
				parseTranslate(tag);
			}
		}
		else if (tagName.compare(ROTATE) == 0) {
			if (modelsInContainer.top() == true) {
				errorString.append("Error: Misplaced rotate tag!\n");

				invalidDoc = true;
			}
			else {
				parseRotate(tag);
			}
		}
		else if (tagName.compare(SCALE) == 0) {
			if (modelsInContainer.top() == true) {
				errorString.append("Error: Misplaced scale tag!\n");
				invalidDoc = true;
			}
			else {
				parseScale(tag);
			}
		}
		else {
			invalidDoc = true;
		}
	}


	void parseDocument(void)
	{
		// Estrutura auxiliar para realizar o parsing do documento xml
		TiXmlElement* scene = doc.FirstChildElement();

		// Testa se existe uma tag chamada scene
		if (scene == NULL || toLower(scene->Value()).compare(SCENE) != 0) {
			invalidDoc = true;
			errorString.append("Failed to load file. No scene element!\n");
			failedModels++;
		}
		else {
			numModelsInContainer.push(0);
			modelsInContainer.push(false);

			// Processa cada um dos modelos
			for (TiXmlElement* tag = scene->FirstChildElement(); 
			     tag != NULL && invalidDoc == false; 
			     tag = tag->NextSiblingElement()) {
				
				parseTag(tag);
			}
		}
	}


	bool openXMLFile(void)
	{
		bool ret = true;

		if (!doc.LoadFile(fileName.c_str())) {
			errorString.append(doc.ErrorDesc());
			ret = false;
		}

		return ret;
	}


public:


	XMLParserImpl(void)
	{
		invalidDoc = false;
		numModels = failedModels = 0;
		errorString = "";
		fileName = "";
	}


	XMLParserImpl(std::string fileName) 
	{
		invalidDoc = false;
		numModels = failedModels = 0;
		errorString = "";
		this->fileName = fileName;

		if (openXMLFile() == true) {
			parseDocument();
		}
	}


	size_t getNumModels(void)
	{
		return numModels;
	}


	size_t getFailedModels(void)
	{
		return failedModels;
	}


	std::vector<GLOperation*> getGLOperations(void)
	{
		if (invalidDoc == true) {
			clearGLOperations();
		}

		return glOperations;
	}


	std::string getErrorString(void)
	{
		return errorString;
	}


	std::string getFileName(void)
	{
		return fileName;
	}


	~XMLParserImpl(void) = default;
};


XMLParser::XMLParser(void)
{
	pimpl = new XMLParserImpl();
}


XMLParser::XMLParser(std::string fileName)
{
	pimpl = new XMLParserImpl(fileName);
}


XMLParser::XMLParser(const XMLParser& x)
{
	pimpl = new XMLParserImpl(x.pimpl->getFileName());
}


size_t XMLParser::getNumModels(void) 
{
	return pimpl->getNumModels();
}


size_t XMLParser::getFailedModels(void)
{
	return pimpl->getFailedModels();
}


std::vector<GLOperation*> XMLParser::getGLOperations(void)
{
	return pimpl->getGLOperations();
}


std::string XMLParser::getErrorString(void)
{
	return pimpl->getErrorString();
}


XMLParser::~XMLParser(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}