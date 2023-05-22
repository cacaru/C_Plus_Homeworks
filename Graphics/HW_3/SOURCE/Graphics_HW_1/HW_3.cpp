#include "glSetup.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>

using namespace std;

#define _USE_MATH_DEFINES
#include <math.h>

void init();
void torus();
void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);

//Camera configuration
vec3 eye(1.5, 2, 1.5);
vec3 center(0, 0, 0);
vec3 up(0, 1, 0);

//Light configuration
vec4 light(1, 2, 1, 0);

//position saver
vec3 p[36][18];
vec3 np[36][18];	//for normal vecter of quads

//texture saver
GLubyte texture1[512][512][3];
GLubyte texture2[512][512][3];
GLubyte texture3[512][512][3];

int selection = 1; // 초기값 

//Global cordinate frame
float AXIS_LENGTH = 2;
float AXIS_LINE_WIDTH = 2;

//Colors
GLfloat bgColor[4] = { 1,1,1,1 };

int main(int argc, char* argv[]) {
	//Initialize OpenGL
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	//CallBack
	glfwSetKeyCallback(window, keyboard);

	//Depth test
	glEnable(GL_DEPTH_TEST);

	//Normalize after transformation
	glEnable(GL_NORMALIZE);

	//Viewport and perspective setting
	reshape(window, windowW, windowH);

	init();
	torus();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		render(window);
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void torus() {
	float radius = 0.2;	 // xy평면에 그린 원의 반지름
	float yradius = 0.3;	// zx평면에 그린 원의 반지름
	for (int j = 0; j < 36; j++) {
		float yangle = j * M_PI * 2 / 36;	// y축 중심 회전 각
		for (int i = 0; i < 18; i++) {
			float angle = i * M_PI * 2 / 18;	// z축 중심 회전 각

			float x = (yradius + radius * cos(angle) + 0.3) * cos(yangle);
			float y = radius * sin(angle) + 0.3;
			float z = (yradius + radius * cos(angle) + 0.3) * sin(yangle);
			p[j][i] = vec3(x, y, z);
		}
	}

	//calculate Normal vector
	for (int j = 0; j < 36; j++) {
		for (int i = 0; i < 18; i++) {
			vec3 v1 = p[(36 - j) % 36][(18 - i - 1) % 18] - p[(36 - j) % 36][(18 - i) % 18];
			vec3 v2 = p[(36 - j - 1) % 36][(18 - i) % 18] - p[(36 - j) % 36][(18 - i) % 18];
			np[j][i] = cross(v1, v2);

			np[j][i] = normalize(np[j][i]);
		}
	}
}


void checkTextureload() {
	FILE* infile;
	fopen_s(&infile, "./Material/check.raw", "rb");
	if (infile == NULL) {
		cout << "read 1 fail" << endl;
		return;
	}
	fread(texture1, sizeof(char), 512 * 512 * 3, infile);
	fclose(infile);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void marbleTextureload() {

	FILE* infile;
	fopen_s(&infile, "./Material/marble.raw", "rb");
	if (infile == NULL) {
		cout << "read 2 fail" << endl;
		return;
	}
	fread(texture2, sizeof(char), 512 * 512 * 3, infile);
	fclose(infile);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void woodTextureload() {

	FILE* infile;
	fopen_s(&infile, "./Material/wood.raw", "rb");
	if (infile == NULL) {
		cout << "read 3 fail" << endl;
		return;
	}
	fread(texture3, sizeof(char), 512 * 512 * 3, infile);
	fclose(infile);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, texture3);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

GLuint texID[3];

void init() {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(3, texID);

	glBindTexture(GL_TEXTURE_2D, texID[0]);
	marbleTextureload();

	glBindTexture(GL_TEXTURE_2D, texID[1]);
	woodTextureload();

	glBindTexture(GL_TEXTURE_2D, texID[2]);
	checkTextureload();


	// keyboard 
	cout << endl;
	cout << "keyboard input : press 1 to change marble texture" << endl;
	cout << "keyboard input : press 2 to change wood texture" << endl;
	cout << "keyboard input : press 3 to change check texture" << endl;
	cout << endl;
	cout << "keyboard input : q or ESC to exit this" << endl;
}

void setupLight() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[4] = { 1, 1, 1, 1 };
	GLfloat diffuse[4] = { 1, 1, 1, 1 };
	GLfloat specular[4] = { 1.0, 1.0, 1.0, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(light));
}

void setupMateral() {
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mat_ambient[4] = { 1,1,1, 1 };
	GLfloat mat_specular[4] = { 1,1,1, 1 };
	GLfloat mat_shininess = 5;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void drawTorus() {
	glBindTexture(GL_TEXTURE_2D, texID[selection - 1]);
	vec2 texCoordi[4];
	texCoordi[0] = vec2(0, 0);
	texCoordi[1] = vec2(1, 0);
	texCoordi[2] = vec2(1, 1);
	texCoordi[3] = vec2(0, 1);
	int x, y;
	glBegin(GL_QUADS);
	for (int j = 0; j < 36; j++) {
		
		for (int i = 0; i < 18; i++) {

			glNormal3fv(value_ptr(np[j % 36][i % 18]));

			glTexCoord2fv(value_ptr(vec2(0.028 * j, 0.056 * i)));
			glVertex3fv(value_ptr(p[j % 36][i % 18]));

			glTexCoord2fv(value_ptr(vec2(0.028 * (j + 1), 0.056 * i)));
			glVertex3fv(value_ptr(p[(j+1) % 36][i % 18]));

			glTexCoord2fv(value_ptr(vec2(0.028 * (j+1), 0.056 * (i+1))));
			glVertex3fv(value_ptr(p[(j+1) % 36][(i+1) % 18]));

			glTexCoord2fv(value_ptr(vec2(0.028 * j, 0.056 * (i+1))));
			glVertex3fv(value_ptr(p[j % 36][(i+1) % 18]));
			
		}
	}
	glEnd();
	
}

void render(GLFWwindow* window) {
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	//Smooth shading
	glShadeModel(GL_SMOOTH);

	setupLight();
	setupMateral();
	
	switch (selection) {
		case 1: drawTorus(); break;
		case 2: drawTorus(); break;
		case 3: drawTorus(); break;
	}
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;

		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		}
	}
}