
#include <iostream>
#include <fstream>
#include <vector>

#include "xmlParser.h"
#include "invalidDocumentException.h"


class XMLParser::XMLParserImpl {
	std::string fileName;
	std::vector<std::string> lines;
	
public:

	XMLParserImpl(void)
	{
		fileName = "";
	}


	XMLParserImpl(std::string fileName)
	{
		this->fileName = fileName;
	}


	std::string getFileName()
	{
		return this->fileName;
	}


	std::vector<std::string> getLines()
	{
		return this->lines;
	}


	//Substituir void por XMLDOM
	void openDocument()
	{
		std::ifstream doc;		//ler como input
		doc.open(this->fileName);
		std::string line;


		if (doc.is_open) {
			while (getline(doc, line)) {
				lines.push_back(line);
			}
		}
		else {
			throw new InvalidDocumentException();	//Verificar métodos da Excepção!
		}
	
		parseLines();
	}


	void parseLines(void)
	{
		//verificar se a linha contém translate, rotate, etc
		//mandar para o método apropriado
		//mandar excepção se for necessário
	}


	void parseTranslate(std::string line)
	{
		std::vector<std::string> fields = splitFields(line, ' ');
		std::vector<std::string> coordinates; //no mínimo é 1 coordenada e no máximo são 3
		bool ret = false;

		//translate + 3 coordenadas + />
		if (fields.size >= 2 && fields.size <= 5) {
			ret = true;
		}

		if (ret && (fields.at(0).compare("<translate") != 0) && (fields.at(fields.size() - 1).compare("/>"))) {
			ret = false;
		}


		for (int i = 1; i < fields.size - 1; i++) {
			coordinates.push_back(fields.at(i));
		}

		if (ret && !checkCoordinates(coordinates)) {
			ret = false;
		}


		if (ret == false) {
			throw new InvalidDocumentException();
		}

	}


	bool checkCoordinates(std::vector<std::string> coordinates)
	{
		bool ret = true;
		size_t size = coordinates.size();
		std::string ;


		for (size_t i = 0; ret && (i < size - 1); i++) {
			if (coordinates.at(i).at(0) == coordinates.at(i + 1).at(0)) {
				ret = false;
			}
		}


	}

	std::vector<std::string> splitFields(std::string& line, char ch)
	{
		std::vector<std::string> fields;
		std::string next;

		for (std::string::const_iterator it = line.begin(); it != line.end(); it++) {

			if (*it == ch) {

				if (!next.empty()) {
					fields.push_back(next);
					next.clear();
				}
			}
			else {
				next += *it;
			}
		}

		if (!next.empty()) {
			fields.push_back(next);
		}

		return fields;
	}

};

XMLParser::XMLParser(void) : pimpl{ new XMLParserImpl() } {}

XMLParser::XMLParser(std::string fileName)
{
	pimpl = new XMLParserImpl(fileName);
}

std::string XMLParser::getFileName(void)
{
	return pimpl->getFileName;
}

std::vector<std::string> XMLParser::getLines(void)
{
	return pimpl->getLines;
}
