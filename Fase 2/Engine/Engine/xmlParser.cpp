/**
 * @file xmlParser.cpp
 *
 * Definição de todos os métodos para se efetuar o parsing de um ficheiro xml
 * que contenha múltiplas instruções a ser executadas em OpenGL.
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 29-03-2017
 */


#include <stack>
#include <algorithm>
#include <map>
#include <stdexcept>
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


/* Definição de constantes e atributos que caracterizam operações em OpenGL */
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
	bool invalidDoc;      // Variável para detetar se ocorreu algum erro de 
	                      // parsing
	size_t numModels;     // Número total de modelos lidos
	size_t failedModels;  // Número total de modelos que não se conseguiram 
	                      // processar

	std::stack<bool> modelsInContainer;       // Stack para informar se uma 
	                                          // scene ou um group já possui, 
	                                          // ou não, uma tag models  
	std::stack<size_t> numModelsInContainer;  // Stack que possui a informação
	                                          // acerca do número de tags 
	                                          // models numa scene ou num group

	std::vector<GLOperation*> glOperations;   // Vetor de operações em OpenGL
	std::string errorString;                  // Representação textual de
	                                          // eventuais erros, ou warnings,
	                                          // que possam ocorrer
	std::string fileName;                     // Nome do ficheiro xml

	// Map que associa o nome de um ficheiro a um conjunto de vértices
	std::map<std::string, std::vector<Vertex>> mapFileVertices;


	/**
	 * Limpa o conteúdo do vetor de operações em OpenGL.
	 */
	void clearGLOperations(void)
	{
		for (size_t i = 0; i < glOperations.size(); i++) {
			delete glOperations.at(i);
		}

		glOperations.clear();
	}


	/**
	 * Converte todos os carateres de uma string para minúsculas.
	 *
	 * @param str String a ser convertida.
	 * @return Devolve uma string que contenha apenas minúsculas.
	 */
	std::string toLower(const char* str)
	{
		std::string aux(str);

		std::transform(aux.begin(), aux.end(), aux.begin(), ::tolower);

		return aux;
	}


	/**
	 * Incrementa o número total de tags models no topo da stack 
	 * numModelsInContainer e modifica o topo da stack modelsInContainer para
	 * true (já possui um tag models).
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
	 * Abre o ficheiro que contém um conjunto de vértices de uma primitiva.
	 * Este ficheiro é aberto apenas uma vez.
	 *
	 * @param fileName Nome do ficheiro.
	 * @return Devolve um vetor com os vértices lidos do ficheiro de um modelo.
	 */
	std::vector<Vertex> readVertices(const char* file)
	{
		std::fstream fp;       // Estrutura para abertura de um ficheiro em
		                       // modo de leitura
		std::string line;      // String auxiliar que irá corresponder a uma 
		                       // linha do ficheiro
		std::stringstream ss;  // String auxiliar para converter texto em 
		                       // valores numéricos

		std::vector<Vertex> vertices;  // Vetor de vértices

		float x = 0;  // Coordenada x de um vértice
		float y = 0;  // Coordenada y de um vértice
		float z = 0;  // Coordenada z de um vértice

		// Extrai do mapa de ficheiros e vértices, os vértices associados
		// à string file, caso estes existam
		try {
			vertices = mapFileVertices.at(file);
		}
		catch (std::out_of_range) {
			// Caso o map não possua esse ficheiro deve-se proceder à abertura
			// do ficheiro e respetiva leitura de vértices

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

				std::string aux(file);   // String auxliar para a conversão

				// O ficheiro lido e conjunto de vértices são adicionados ao
				// mapa. Assim, não será necessário proceder-se à leitura deste
				// ficheiro novamente
				mapFileVertices.insert(
					std::pair<std::string, std::vector<Vertex>>(file, vertices));
			}
			else {
				errorString.append("Warning: Could not read file ");
				errorString.append(file);
				errorString.append("!\n");

				// Se não se conseguiu abrir bem o ficheiro então o modelo atual 
				// conta como um modelo que não pode ser processado
				failedModels++;
			}
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
		std::vector<Vertex> vertices;  // Vetor de vértices de um modelo.
		size_t numFiles = 0;           // Número de ficheiros associados à tag
		                               // model

		float diffR = 0;  // Valor da difusão em vermelho 
		float diffG = 0;  // Valor da difusão em verde 
		float diffB = 0;  // Valor da difusão em azul 

		size_t numColors = 0;  // Número de atributos relativamente a cores

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
				numColors++;
			}
			else if (attName.compare(DIFFG) == 0) {
				std::stringstream ss(a->Value());
				ss >> diffG;
				numColors++;
			}
			else if (attName.compare(DIFFB) == 0) {
				std::stringstream ss(a->Value());
				ss >> diffB;
				numColors++;
			}
			else {
				errorString.append("Error: Invalid model attribute!\n");
				invalidDoc = true;
			}
		}

		// A cor de um modelo é branca por defeito
		if (numColors == 0) {
			diffR = diffG = diffB = 1;
		}

		glOperations.push_back(new TrianglesDrawing(vertices, 
			                                        diffR, diffG, diffB));
	}


	/**
	 * Efetua o parsing dos atributos associados à tag models.
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
	 * Sempre que se inicia um group é efetuada uma pushMatrix. No final do 
	 * tratamento de todos os elementos de um group é efetuada uma popMatrix.
	 *
	 * @param group Tag group.
	 */
	void parseGroup(TiXmlElement* group)
	{
		glOperations.push_back(new PushMatrix());

		modelsInContainer.push(false);
		numModelsInContainer.push(0);

		for (TiXmlElement* tag = group->FirstChildElement(); 
		     tag != NULL && invalidDoc == false; 
		     tag = tag->NextSiblingElement()) {
			
			parseTag(tag);
		}

		// Como já se processaram os elementos de um group então remove-se
		// a informação deste nas stacks auxiliares
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

			// Se o número de models tags for igual ou superior a 1 então o
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

		// Não faz sentido haver rotates, translates ou scales no final de um
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

		// Apenas são admitidas tags dos tipos mencionados acima
		else {
			errorString.append("Error: Invalid tag!\n");
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

			glOperations.push_back(new PushMatrix());

			// Processa cada um dos modelos
			for (TiXmlElement* tag = scene->FirstChildElement(); 
			     tag != NULL && invalidDoc == false; 
			     tag = tag->NextSiblingElement()) {
				
				parseTag(tag);
			}

			glOperations.push_back(new PopMatrix());
		}
	}


	/**
	 * Abre um documento xml para ser efetuado o seu parsing.
	 * 
	 * @return Devolve true caso o ficheiro tenha sido aberto com sucesso ou 
	 *         false caso contrário.
	 */
	bool openXMLFile(void)
	{
		bool ret = true;

		if (!doc.LoadFile(fileName.c_str())) {
			errorString.append("Could not read file ");
			errorString.append(fileName);
			errorString.append("...\n");
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
	 * Construtor por parâmetros.
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
	 * Devolve o número total de modelos lidos do ficheiro xml.
	 */
	size_t getNumModels(void)
	{
		return numModels;
	}


	/**
	 * Devolve o número de modelos que não se conseguiu processar.
	 */
	size_t getFailedModels(void)
	{
		return failedModels;
	}


	/**
	 * Devolve o vetor com o conjunto de operações em OpenGL.
 	 */
	std::vector<GLOperation*> getGLOperations(void)
	{
		if (invalidDoc == true) {

			// Se houve algum erro são limpas eventuais operações que já
			// tenham sido armazendas no vetor
			clearGLOperations();
		}

		return glOperations;
	}


	/**
	 * Devolve uma representação textual de eventuais erros, ou warnings, do
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
 * Construtor por parâmetros.
 *
 * @param fileName Nome do ficheiro.
 */
XMLParser::XMLParser(std::string fileName)
{
	pimpl = new XMLParserImpl(fileName);
}


/**
 * Construtor de cópia.
 *
 * @param x Objeto da classe XMLParser a copiar.
 */
XMLParser::XMLParser(const XMLParser& x)
{
	pimpl = new XMLParserImpl(x.pimpl->getFileName());
}


/**
 * Devolve o número total de modelos lidos do ficheiro xml.
 */
size_t XMLParser::getNumModels(void) 
{
	return pimpl->getNumModels();
}


/**
 * Devolve o número de modelos que não se conseguiu processar.
 */
size_t XMLParser::getFailedModels(void)
{
	return pimpl->getFailedModels();
}


/**
 * Devolve o vetor com o conjunto de operações em OpenGL.
 */
std::vector<GLOperation*> XMLParser::getGLOperations(void)
{
	return pimpl->getGLOperations();
}


/**
 * Devolve uma representação textual de eventuais erros, ou warnings, do
 * parsing do ficheiro xml, caso estes existam.
 */
std::string XMLParser::getErrorString(void)
{
	return pimpl->getErrorString();
}


/**
 * Destrutor da classe XMLParser. Liberta a memória ocupada pelo apontador para
 * implementação da classe XMLParser.
 */
XMLParser::~XMLParser(void)
{
	if (pimpl != NULL) {
		delete pimpl;
		pimpl = NULL;
	}
}