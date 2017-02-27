
#include "invalidDocumentException.h"

#include <iostream>
#include <string>

using namespace std;

class InvalidDocumentException {

public:
	InvalidDocumentException(void) : msgXML() {}

	InvalidDocumentException(const string& msg) : msgXML(msg) {}
	~InvalidDocumentException() {}

	string getMessage() const { return(msgXML); }

private:
	string msgXML = "Invalid XML document.";
};