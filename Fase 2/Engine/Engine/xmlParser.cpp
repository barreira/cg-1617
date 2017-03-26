/**
 * @file xmlParser.cpp
 *
 * Defini��o de todos os m�todos para se efetuar o parsing de um ficheiro xml
 * que contenha m�ltiplas instru��es a ser executadas em OpenGL.
 *
 * @author Carlos Pereira - A61887
 * @author Jo�o Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 26-03-2017
 */


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


/* Defini��o de constantes e atributos que caracterizam opera��es em OpenGL */
#define SCENE  "scene"
#define GROUP  "group"
#define MODELS "models"

/* Tag de model e atributos */
#define MODEL "model"
#define FILE  "file"
#define DIFFR "diffr"
#define DIFFG "diffg"
#define DIFFB "diffb"

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
	TiXmlDocument doc;    // Documento tinyXML 
	bool invalidDoc;      // Vari�vel para detetar se ocorreu algum erro de 
	                      // parsing
	size_t numModels;     // N�mero total de modelos lidos
	size_t failedModels;  // N�mero total de modelos que n�o se conseguiram 
	                      // processar

	std::stack<bool> modelsInContainer;       // Stack para informar se uma 
	                                          // scene ou um group j� possui, 
	                                          // ou n�o, uma tag models  
	std::stack<size_t> numModelsInContainer;  // Stack que possui a informa��o
	                                          // acerca do n�mero de tags 
	                                          // models numa scene ou num group

	std::vector<GLOperation*> glOperations;   // Vetor de opera��es em OpenGL
	std::string errorString;                  // Representa��o textual de
	                                          // eventuais erros, ou warnings,
	                                          // que possam ocorrer
	std::string fileName;                     // Nome do ficheiro xml


	/**
	 * Limpa o conte�do do vetor de opera��es em OpenGL.
	 */
	void clearGLOperations(void)
	{
		for (size_t i = 0; i < glOperations.size(); i++) {
			delete glOperations.at(i);
		}

		glOperations.clear();
	}


	/**
	 * Converte todos os carateres de uma string para min�sculas.
	 *
	 * @param str String a ser convertida.
	 * @return Devolve uma string que contenha apenas min�sculas.
	 */
	std::string toLower(const char* str)
	{
		std::string aux(str);

		std::transform(aux.begin(), aux.end(), aux.begin(), ::tolower);

		return aux;
	}


	/**
	 * Incrementa o n�mero total de tags models no topo da stack 
	 * numModelsInContainer e modifica o topo da stack modelsInContainer para
	 * true (j� possui um tag models).
	 */
	void addModelsInContainer(void)
	{
		size_t aux = numModelsInContainer.top();

		numModelsInContainer.pop();
		numModelsInContainer.push(++aux);
		modelsInContainer.pop();
		modelsInContainer.push(true);
	}



	/**
	 * Abre o ficheiro que cont�m um conjunto de v�rtices de uma primitiva.
	 * Este ficheiro � aberto apenas uma vez.
	 *
	 * @param fileName Nome do ficheiro.
	 * @return Devolve um vetor com os v�rtices lidos do ficheiro de um modelo.
	 */
	std::vector<Vertex> readVertices(const char* file)
	{
		std::fstream fp;       // Estrutura para abertura de um ficheiro em
		                       // modo de leitura
		std::string line;      // String auxiliar que ir� corresponder a uma 
		                       // linha do ficheiro
		std::stringstream ss;  // String auxiliar para converter texto em 
		                       // valores num�ricos

		std::vector<Vertex> vertices;  // Vetor de v�rtices

		float x = 0;  // Coordenada x de um v�rtice
		float y = 0;  // Coordenada y de um v�rtice
		float z = 0;  // Coordenada z de um v�rtice

		fp.open(file);

		// Testa se o ficheiro foi bem aberto e inicia a sua leitura
		if (fp.is_open()) {

			// L� linha a linha
			while (getline(fp, line)) {
				std::stringstream ss(line);

				// Converte uma linha para as coordenadas x, y e z
				ss >> x >> y >> z;

				// Adiciona as coordenadas ao vetor de v�rtices
				vertices.push_back(Vertex(x, y, z));
			}

			fp.close();
		}
		else {
			errorString.append("Warning: Could not read file ");
			errorString.append(file);
			errorString.append("!\n");

			// Se n�o se conseguiu abrir bem o ficheiro ent�o o modelo atual 
			// conta como um modelo que n�o pode ser processado
			failedModels++;
		}

		return vertices;
	}


	/**
	 * Efetua o parsing de um modelo.
	 *
	 * @param model Tag model.
	 */
	void parseModel(TiXmlElement* model)
	{
		std::vector<Vertex> vertices;  // Vetor de v�rtices de um modelo.
		size_t numFiles = 0;           // N�mero de ficheiros associados � tag
		                               // model

		// A cor de um modelo � preta por defeito
		float diffR = 0;  // Valor da difus�o em vermelho 
		float diffG = 0;  // Valor da difus�o em verde 
		float diffB = 0;  // Valor da difus�o em azul 

		numModels++;

		for (TiXmlAttribute* a = model->FirstAttribute(); 
		     a != NULL && invalidDoc == false; a = a->Next()) {
			
			std::string attName(toLower(a->Name()));

			if (attName.compare(FILE) == 0) {
				if (numFiles > 0) {

					// Um modelo apenas pode conter um ficheiro
					invalidDoc = true;
				}
				else {
					vertices = readVertices(a->Value());
					numFiles++;
				}
			}
			else if (attName.compare(DIFFR) == 0) {
				std::stringstream ss(a->Value());
				ss >> diffR;
			}
			else if (attName.compare(DIFFG) == 0) {
				std::stringstream ss(a->Value());
				ss >> diffG;
			}
			else if (attName.compare(DIFFB) == 0) {
				std::stringstream ss(a->Value());
				ss >> diffB;
			}
			else {
				errorString.append("Error: Invalid model attribute!\n");
				invalidDoc = true;
			}
		}

		glOperations.push_back(new TrianglesDrawing(vertices, 
			                                        diffR, diffG, diffB));
	}


	/**
	 * Efetua o parsing dos atributos associados � tag models.
	 *
	 * @param models Tag models.
	 */
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

				// Uma tag models apenas pode ter a si associada filhos do tipo
				// model
				errorString.append("Error: Invalid models tag!\n");
				invalidDoc = true;
			}
		}
	}


	/**
	 * Efetua o parsing de um group.
	 *
	 * Sempre que se inicia um group � efetuada uma pushMatrix. No final do 
	 * tratamento de todos os elementos de um group � efetuada uma popMatrix.
	 *
	 * @param group Tag group.
	 */
	void parseGroup(TiXmlElement* group)
	{
		glOperations.push_back(new PushMatrix());

		for (TiXmlElement* tag = group->FirstChildElement(); 
		     tag != NULL && invalidDoc == false; 
		     tag = tag->NextSiblingElement()) {
			
			parseTag(tag);
		}

		// Como j� se processaram os elementos de um group ent�o remove-se
		// a informa��o deste nas stacks auxiliares
		numModelsInContainer.pop();
		modelsInContainer.pop();

		glOperations.push_back(new PopMatrix());
	}


	/**
	 * Efetua o parsing de uma translate.
	 *
     * @param translate Tag translate.
	 */
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


	/**
	 * Efetua o parsing de uma rotate.
	 *
	 * @param rotate Tag rotate.
	 */
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


	/**
	 * Efetua o parsing de uma scale.
	 *
	 * @param scale Tag scale.
	 */
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


	/**
	 * Testa a que tipo pertence uma tag e para cada tipo efetua o seu 
	 * respetivo parsing.
	 *
	 * @param tag Tag recebida.
	 */
	void parseTag(TiXmlElement* tag)
	{
		std::string tagName = toLower(tag->Value());

		// Testa se existe uma tag chamada model
		if (tagName.compare(MODELS) == 0) {

			// Se o n�mero de models tags for igual ou superior a 1 ent�o o
			// parsing termina
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

		// N�o faz sentido haver rotates, translates ou scales no final de um
		// group ou de uma scene
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

		// Apenas s�o admitidas tags dos tipos mencionados acima
		else {
			invalidDoc = true;
		}
	}


	/**
	 * Efetua o parsing de um documento xml.
	 */
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


	/**
	 * Abre um documento xml para ser efetuado o seu parsing.
	 * 
	 * @return Devolve true caso o ficheiro tenha sido aberto com sucesso ou 
	 *         false caso contr�rio.
	 */
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


	/**
	 * Construtor por defeito.
	 */
	XMLParserImpl(void)
	{
		invalidDoc = false;
		numModels = failedModels = 0;
		errorString = "";
		fileName = "";
	}


	/**
	 * Construtor por par�metros.
	 *
	 * @param fileName Nome do ficheiro.
	 */
	XMLParserImpl(std::string fileName) 
	{
		invalidDoc = false;
		numModels = failedModels = 0;
		errorString = "";
		this->fileName = fileName;

		// Abre o ficheiro com o nome fileName para ser efetuado o seu parsing
		if (openXMLFile() == true) {
			parseDocument();
		}
	}


	/**
	 * Devolve o n�mero total de modelos lidos do ficheiro xml.
	 */
	size_t getNumModels(void)
	{
		return numModels;
	}


	/**
	 * Devolve o n�mero de modelos que n�o se conseguiu processar.
	 */
	size_t getFailedModels(void)
	{
		return failedModels;
	}


	/**
	 * Devolve o vetor com o conjunto de opera��es em OpenGL.
 	 */
	std::vector<GLOperation*> getGLOperations(void)
	{
		if (invalidDoc == true) {

			// Se houve algum erro s�o limpas eventuais opera��es que j�
			// tenham sido armazendas no vetor
			clearGLOperations();
		}

		return glOperations;
	}


	/**
	 * Devolve uma representa��o textual de eventuais erros, ou warnings, do
	 * parsing do ficheiro xml, caso estes existam.
	 */
	std::string getErrorString(void)
	{
		return errorString;
	}


	/**
	 * Devolve o nome do ficheiro xml que se efetuou o parsing.
	 */
	std::string getFileName(void)
	{
		return fileName;
	}


	/**
	 * Destrutor (por defeito em c++).
	 */
	~XMLParserImpl(void) = default;
};


/**
 * Construtor por defeito.
 */
XMLParser::XMLParser(void)
{
	pimpl = new XMLParserImpl();
}


/**
 * Construtor por par�metros.
 *
 * @param fileName Nome do ficheiro.
 */
XMLParser::XMLParser(std::string fileName)
{
	pimpl = new XMLParserImpl(fileName);
}


/**
 * Construtor de c�pia.
 *
 * @param x Objeto da classe XMLParser a copiar.
 */
XMLParser::XMLParser(const XMLParser& x)
{
	pimpl = new XMLParserImpl(x.pimpl->getFileName());
}


/**
 * Devolve o n�mero total de modelos lidos do ficheiro xml.
 */
size_t XMLParser::getNumModels(void) 
{
	return pimpl->getNumModels();
}


/**
 * Devolve o n�mero de modelos que n�o se conseguiu processar.
 */
size_t XMLParser::getFailedModels(void)
{
	return pimpl->getFailedModels();
}


/**
 * Devolve o vetor com o conjunto de opera��es em OpenGL.
 */
std::vector<GLOperation*> XMLParser::getGLOperations(void)
{
	return pimpl->getGLOperations();
}


/**
 * Devolve uma representa��o textual de eventuais erros, ou warnings, do
 * parsing do ficheiro xml, caso estes existam.
 */
std::string XMLParser::getErrorString(void)
{
	return pimpl->getErrorString();
}


/**
 * Destrutor da classe XMLParser. Liberta a mem�ria ocupada pelo apontador para
 * implementa��o da classe XMLParser.
 */
XMLParser::~XMLParser(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}