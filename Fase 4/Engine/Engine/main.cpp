/**
 * @file main.cpp
 *
 * @brief Definição de todos métodos para o funcionamento do programa Engine.
 *
 * @author Carlos Pereira - A61887
 * @author João Barreira  - A73831
 * @author Rafael Braga   - A61799
 *
 * @version 12-05-2017 
 */


#include <cstdlib>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "xmlParser.h"


// Ficheiro xml por defeito
#define XML_FILE   "demos/cone.xml" 

// Background por defeito
#define BACKGROUND "demos/univ.jpg"


#define FRONT_AND_BACK 1  // Desenhar a parte da frente e de trás de
                          // uma primitiva

const float PI = 3.14159265358979323846f;  // Valor da constante pi


float xPos = 0.0f;    // Posição x
float zPos = 0.0f;    // Posição z
float angleX = 0.0f;  // Ângulo em relação ao eixo dos xx
float angleY = 0.0f;  // Ângulo em relação ao eixo dos yy


// Por defeito o modo de desenho é wired
GLenum mode = GL_FILL;

// Por defeito desenham-se ambos os lados de uma primitiva
GLenum drawMode = GL_BACK;


float lx = 0.0f;               // Posição para onde se está a olhar no eixo dos
                               // xx
float px = 0.0f;               // Posição da câmara no eixo dos xx

float ly = 0.0f;               // Posição para onde se está a olhar no eixo dos
                               // yy
float py = 0.0f;               // Posição da câmara no eixo dos yy

float lz = -1.0f;              // Posição para onde se está a olhar no eixo dos
                               // zz
float pz = 15.0f;              // Posição da câmara no eixo dos zz

float cameraAngleX = 0.0f;     // Ângulo da câmara no eixo dos xx
float deltaAngleX = 0.0f;      // Ângulo para cálculos auxilares
int xOrigin = -1;              // Posição x do rato

float cameraAngleY = 0.0f;     // Ângulo da câmara no eixo dos yy
float deltaAngleY = 0.0f;      // Ângulo para cálculos auxiliares
int yOrigin = -1;              // Posição y do rato

const float vCameraX = 0.003f; // Velocidade de rotação da câmara em X
const float vCameraY = 0.5f;   // Velocidade de rotação da câmara em Y


std::vector<GLOperation*> glOperations;  // Vetor de operações em OpenGL

GLuint texID;
std::string background;

/**
 * Função responsável pela representação de uma cena em OpenGL. Percorre
 * o vetor de operações em openGL (translações, rotações, desenho de 
 * triângulos, ...) e executa cada uma destas.
 */
void drawScene(void)
{	
	for (size_t i = 0; i < glOperations.size(); i++) {
		glOperations.at(i)->execute();
	}
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


void drawBackground(void)
{
	float w = 0.0f;
	float h = 0.0f;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	w = (float)glutGet(GLUT_WINDOW_WIDTH);
	h = (float)glutGet(GLUT_WINDOW_HEIGHT);

	gluOrtho2D(0.0, (GLdouble)w, (GLdouble)h, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texID);

	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, h, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(w, h, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(w, 0.0f, 0.0f);

	glEnd();

	glPopMatrix(); 
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
		      px + lx, py + ly, pz + lz,
		      0.0f, 1.0f, 0.0f);

	drawBackground();

	glPushMatrix();

	glTranslatef(xPos, 0.0f, zPos);
	glRotatef(angleY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleX, 1.0f, 0.0f, 0.0f);


	if (drawMode != FRONT_AND_BACK) {
		glEnable(GL_CULL_FACE);
		glCullFace(drawMode);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	glPolygonMode(GL_FRONT_AND_BACK, mode);

	// Representa a cena correspondente a um conjunto de operações em OpenGL
	drawScene();

	glPopMatrix();

	glutSwapBuffers();
}


/**
 * Função que trata dos eventos do teclado, teclas 'w', 's', 'a' e 'd'.
 */
void keyboardEvent(unsigned char key, int x, int y)
{
	if (key == 'w') {

		// Afasta um modelo
		zPos -= 0.3f;
	}
	else if (key == 's') {

		// Aproxima um modelo
		zPos += 0.3f;
	}
	else if (key == 'a') {

		// Move um modelo para a direita
		xPos -= 0.3f;
	}
	else if (key == 'd') {

		// Move um modelo para a esquerda
		xPos += 0.3f;
	}
}


/**
 * Função que trata dos eventos respetivos às arrow keys.
 */
void rotateEvent(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) {

		// Roda o modelo em torno do eixo dos yy no sentido dos 
		// ponteiros do relógio
		angleY -= 3.0f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		
		// Roda o modelo em torno do eixo dos yy no sentido contrário ao dos 
		// ponteiros do relógio
		angleY += 3.0f;
	}
	else if (key == GLUT_KEY_UP) {

		// Roda o modelo em torno do eixo dos yy no sentido dos 
		// ponteiros do relógio
		angleX -= 3.0f;
	}
	else if (key == GLUT_KEY_DOWN) {

		// Roda o modelo em torno do eixo dos yy no sentido contrário ao dos 
		// ponteiros do relógio
		angleX += 3.0f;
	}
}


/**
 * Função que trata das opções selecionadas no menu.
 */
void processMenuEvents(int option)
{
	if (option == GL_FILL) {

		// Preenche a primitiva
		mode = GL_FILL;
	}
	else if (option == GL_LINE) {

		// Desenha a primitiva em modo wired
		mode = GL_LINE;
	}
	else if (option == GL_POINT) {

		// Desenha apenas os vértices da primitiva
		mode = GL_POINT;
	}
	else if (option == GL_BACK) {
		drawMode = GL_BACK;
	}
	else if (option == GL_FRONT) {
		drawMode = GL_FRONT;
	}
	else if (option == FRONT_AND_BACK) {
		drawMode = FRONT_AND_BACK;
	}
}


/**
 * Handler para o movimento do rato.
 */
void mouseMove(int x, int y) 
{
	// Apenas se roda a câmara quando o botão do lado esquerdo do rato
	// está a ser pressionado
	if (xOrigin >= 0) {

		// O valor  
		deltaAngleX = (x - xOrigin) * vCameraX;

		// Atualiza-se a direção da câmara
		lx = sin(cameraAngleX + deltaAngleX);
		lz = -cos(cameraAngleX + deltaAngleX);
	}

	if (yOrigin >= 0) {
		deltaAngleY = (y - yOrigin) * vCameraY;

		// Limita-se o ângulo da câmara em Y entre -90 e 90 graus
		if (cameraAngleY + deltaAngleY >= 90.0f) {
			ly = tan(89.0f * PI / 180.0f);
		}
		else if (cameraAngleY + deltaAngleY <= -90.0f) {
			ly = tan(-89.0f * PI / 180.0f);
		}
		else {
			ly = tan((cameraAngleY + deltaAngleY) * PI / 180.0f);
		}
	}
}


/**
 * Handler para os eventos do rato.
 * Atualiza o angulo x e y da câmara.
 */
void mouseButton(int button, int state, int x, int y) 
{
	// A animação começa apenas quando o botão do lado esquerdo do rato é
	// pressionado
	if (button == GLUT_LEFT_BUTTON) {

		// Depois de o botão ser libertado atualizam-se os ângulos da câmara
		if (state == GLUT_UP) {
			cameraAngleX += deltaAngleX;
			xOrigin = -1;
			
			cameraAngleY += deltaAngleY;

			yOrigin = -1;
		}
		else {
			xOrigin = x;
			yOrigin = y;
		}
	}
}


void loadBackground(void)
{
	unsigned int t = 0;
	unsigned int tw = 0;
	unsigned int th = 0;
	unsigned char *texData;

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)background.c_str());

	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}


/**
 * Função que inicializa todos os componentes da glut
 */
void initGlut(int argc, char **argv)
{
	int menu = 0;

	// Cria a janela e centra-a no ecrã
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 4, glutGet(GLUT_SCREEN_HEIGHT) / 4);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2);
	glutCreateWindow("Engine");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Registos de funções
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboardEvent);
	glutSpecialFunc(rotateEvent);
	menu = glutCreateMenu(processMenuEvents);

	// Categorias do menu
	glutAddMenuEntry("GL_FILL", GL_FILL);
	glutAddMenuEntry("GL_LINE", GL_LINE);
	glutAddMenuEntry("GL_POINT", GL_POINT);
	glutAddMenuEntry("DRAW_FRONT", GL_BACK);
	glutAddMenuEntry("DRAW_BACK", GL_FRONT);
	glutAddMenuEntry("DRAW_FRONT_AND_BACK", FRONT_AND_BACK);

	// O menu é acionado sempre que se pressiona o botão direito do rato
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Funções de tratamento dos eventos do rato
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);


#ifndef __APPLE___
	glewInit();
#endif

	ilInit();

	//  OpenGL settings
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_SMOOTH);
	glEnable(GL_RESCALE_NORMAL);
	
	loadBackground();

	// enter GLUT's main cycle
	glutMainLoop();
}


/**
 * Função principal do programa.
 *
 * @param argc Número de argumentos recebidos.
 * @param argv Argumentos recebidos.
 */
int main(int argc, char **argv)
{
	XMLParser* parser;

	std::cout << "Processing models..." << std::endl;

	// Se o número de argumentos for maior que 1 então abre o ficheiro
	// correspondente ao segundo argumento
	if (argc > 1) {
		parser = new XMLParser(argv[1]);
	}
	else {
		// Caso contrário carrega-se o ficheiro por defeito.
		parser = new XMLParser(XML_FILE);
	}

	// Se possuir mais que dois argumentos então o terceiro argumento
	// corresponde à textura do background
	if (argc > 2) {
		background = argv[2];
	}
	else {

		// Caso contrário é carregada a textura por defeito
		background = BACKGROUND;
	}

	glOperations = parser->getGLOperations();

	// Testa-se se o parsing foi efetuado com sucesso
	if (glOperations.size() > 0) {

		// Apenas se inicia a glut se todos os modelos foram processados com
		// sucesso
		if (parser->getFailedModels() < parser->getNumModels()) {
			// Mostra no ecrã eventuais warnings que possam ter ocorrido
			std::cout << parser->getErrorString();
			
			delete parser;
			initGlut(argc, argv);
		}
		else {
			std::cout << "Could not process the models!" << std::endl;
		}
	}
	else {
		std::cout << parser->getErrorString();
	}

	delete parser;

	std::cout << "Press any key to continue..." << std::endl;

	getchar();
	return 1;
}