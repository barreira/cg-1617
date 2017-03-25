#ifndef xmlParser_h
#define xmlParser_h


#include <string>
#include <vector>
#include "glOperation.h"


class XMLParser {
	class XMLParserImpl;
	XMLParserImpl* pimpl;

public:
	XMLParser(void);


	XMLParser(std::string fileName);


	XMLParser(const XMLParser& x);


	size_t getNumModels(void);


	size_t getFailedModels(void);


	std::vector<GLOperation*> getGLOperations(void);


	std::string getErrorString(void);


	~XMLParser(void);
};


#endif 

