#ifndef xmlParser_h
#define xmlParser_h

#include <string>

class XMLParser {
	class XMLParserImpl;
	XMLParserImpl* pimpl;

public:
	XMLParser(void);
	XMLParser(std::string fileName);
	std::string XMLParser::getFileName(void);
	std::vector<std::string> XMLParser::getLines(void);


	~XMLParser(void);
};

#endif