#ifndef invalidDocumentException_h
#define invalidDocumentException_h


class InvalidDocumentException {

public:
	InvalidDocumentException(void);
	InvalidDocumentException(const std::string& msg);
	
	~InvalidDocumentException(void);
};

#endif