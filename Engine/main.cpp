#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "tinyxml.h"
#include "vertex.h"


#define FILE "input.xml"
#define SCENE "scene"
#define MODEL "model"


TiXmlDocument doc;


float xPos = 0;
float zPos = 0;
float angle = 0;

GLenum mode = GL_FILL;

bool isFileReaded = false;;
std::vector<Vertex> vertices;


void readVertices(const char* fileName)
{
	std::ifstream file;
	std::string line;
	std::stringstream ss;

	float x = 0;
	float y = 0;
	float z = 0;
	
	file.open(fileName);

	if (file.is_open()) {

		while (getline(file, line)) {
			std::stringstream ss(line);

			ss >> x >> y >> z;

			vertices.push_back(Vertex(x, y, z));

			ss.str("");
		}

		file.close();
	}
	else {
		std::cout << "Could not read file " << fileName << "!" << std::endl;
	}
}


void drawVertices(void)
{	
	glBegin(GL_TRIANGLES);

	for (size_t i = 0; i < vertices.size(); i++) {
		glVertex3f(vertices.at(i).getX(),
				   vertices.at(i).getY(),
				   vertices.at(i).getZ());
	}

	glEnd();
}


void draw(void)
{
	TiXmlElement *scene;
	
	if (!isFileReaded) {
		scene = doc.FirstChildElement();

		if (scene == NULL || strcmp(scene->Value(), SCENE) != 0) {
			std::cout << "Failed to load file. No scene element!" << std::endl;

			doc.Clear();
		}

		for (TiXmlElement* elem = scene->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
			std::string elemName = elem->Value();

			if (elemName.compare(MODEL) != 0) {
				std::cout << "Invalid document!" << std::endl;
				break;
			}

			for (TiXmlAttribute* a = elem->FirstAttribute(); a; a = a->Next()) {

				readVertices(a->Value());
			}
		}

		isFileReaded = true;
	}

	drawVertices();
}


void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).

	if (h == 0) {
		h = 1;
	}

	// compute window's aspect ratio 

	float ratio = w * 1.0 / h;

	// Set the projection matrix as current

	glMatrixMode(GL_PROJECTION);

	// Load Identity Matrix

	glLoadIdentity();

	// Set the viewport to be the entire window

	glViewport(0, 0, w, h);

	// Set perspective

	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode

	glMatrixMode(GL_MODELVIEW);
}


void renderScene(void)
{

	// clear buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera

	glLoadIdentity();
	gluLookAt(15.0, 15.0, 15.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put the geometric transformations here

	glTranslatef(xPos, 0, zPos);
	glRotatef(angle, 0, 1, 0);

	// put drawing instructions here


	glPolygonMode(GL_FRONT_AND_BACK, mode);

	draw();

	// End of frame

	glutSwapBuffers();
}



// write function to process keyboard events

void keyboardEvent(unsigned char key, int x, int y)
{
	if (key == 'w') {
		zPos -= 0.1;
	}

	if (key == 's') {
		zPos += 0.1;
	}

	if (key == 'a') {
		xPos -= 0.1;
	}

	if (key == 'd') {
		xPos += 0.1;
	}

	glutPostRedisplay();
}


void rotateEvent(int key, int x, int y)
{

	if (key == GLUT_KEY_LEFT) {
		angle -= 2;
	}

	if (key == GLUT_KEY_RIGHT) {
		angle += 2;
	}

	glutPostRedisplay();
}


// write function to process menu events


void processMenuEvents(int option)
{
	if (option == GL_FILL) {
		mode = GL_FILL;
	}

	if (option == GL_LINE) {
		mode = GL_LINE;
	}

	if (option == GL_POINT) {
		mode = GL_POINT;
	}

	glutPostRedisplay();
}


/*
TiXmlDocument doc;

if (!doc.LoadFile("input.xml")) {
	std::cout << doc.ErrorDesc() << std::endl;

}
else {
	TiXmlElement *root = doc.FirstChildElement();

	if (root == NULL) {
		std::cout << "Failed to load file. No root element." << std::endl;

		doc.Clear();
	}

	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();

		const char* attr;

		if (elemName == "model")
		{
			//attr = elem->Attribute("file");
			attr = elem->FirstAttribute()->Value();
			if (attr != NULL)
				std::cout << attr << std::endl;


		}

	}


}
*/


bool parseDocument(const char* fileName)
{
	bool ret = true;

	if (!doc.LoadFile(fileName)) {
		std::cout << doc.ErrorDesc() << std::endl;
		ret = false;
	}

	return ret;
}


void initGlut(int argc, char **argv)
{
	int menu = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Engine");

	// Required callback registry 

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard and menu callbacks

	glutKeyboardFunc(keyboardEvent);
	glutSpecialFunc(rotateEvent);

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events
	menu = glutCreateMenu(processMenuEvents);

	//add entries to our menu
	glutAddMenuEntry("GL_FILL", GL_FILL);
	glutAddMenuEntry("GL_LINE", GL_LINE);
	glutAddMenuEntry("GL_POINT", GL_POINT);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//  OpenGL settings

	glEnable(GL_DEPTH_TEST);

	// enter GLUT's main cycle

	glutMainLoop();
}


int main(int argc, char **argv)
{
	// init GLUT and the window

	int menu = 0;
	bool isDocOK = true;

	if (argc > 1) {
		isDocOK = parseDocument(argv[1]);
	}
	else {
		isDocOK = parseDocument(FILE);
	}

	if (isDocOK) {
		initGlut(argc, argv);

	}

	return 1;
}

/*

int main(int argc, char** argv)
{
	
	TiXmlDocument doc;

	if (!doc.LoadFile("input.xml")) {
		std::cout << doc.ErrorDesc() << std::endl;
	
	}
	else {
		TiXmlElement *root = doc.FirstChildElement();

		if (root == NULL) {
			std::cout << "Failed to load file. No root element." << std::endl;
		
			doc.Clear();
		}

		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			std::string elemName = elem->Value();

			const char* attr;

			if (elemName == "model")
			{
				//attr = elem->Attribute("file");
				attr = elem->FirstAttribute()->Value();
				if (attr != NULL)
					std::cout << attr << std::endl;


			}

		}


	}
	

	getchar();
	return 0;	
}
*/