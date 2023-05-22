#include "glSetup.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include<iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>

void render(GLFWwindow* window);
void keyboard(GLFWwindow* window, int key, int code, int action, int mods);
void drawCircle();
void torus();
void init();
void drawNormal();

//Camera configuration
vec3 eye(1.5, 1.5, 1.5);
vec3 center(0, 0, 0);
vec3 up(1, 3, 1);

//Light configuration
vec4 light(5.0, 5.0, 5.0, 1);

//Global coordinate frame
float AXIS_LENGTH = 1;
float AXIS_LINE_WIDTH = 2;

//backgrand color
GLfloat bgColor[4] = { 1,1,1,1 };

//position saver
vec3 p[36][18];
vec3 np[36][18];	//for normal vecter of vertex
vec3 cp[36][18];	// vertex to eye

//selection mode
int selection = 1; // default : GL_POINTS
int bfselection = 1;
bool isNormal = false;

// value for arround y-axis sweep 
int ySweep = 1;	//max : 36
int zSweep = 18; // max : 18

int main(int argc, char* argv[]) {
	//orthographics viewing
	perspectiveView = true;

	//Initialize the OpenGl system
	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	//Callbacks to keyboard input
	glfwSetKeyCallback(window, keyboard);

	//Depth test
	glEnable(GL_DEPTH_TEST);

	//Normal vectors are normalized after transformation
	glEnable(GL_NORMALIZE);
	//Offset
	glEnable(GL_POLYGON_OFFSET_LINE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	//viewport and perspective setting
	reshape(window, windowW, windowH);

	torus();			// make torus's point
	init();
	while (!glfwWindowShouldClose(window)) {
		render(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void init() {
	cout << "press button 1 : points" << endl;
	cout << "press button 2 : wireFrame" << endl;
	cout << "press button 3 : blueQuads" << endl;
	cout << "press button 4 : wireFrame and blueQuads" << endl;
	cout << "press button 5 : toggling normalVector" << endl;
	cout << "press button 6 : Two-side shading" << endl;

	cout << endl;
	cout << "press a -> left side move" << endl;
	cout << "press s -> right side move" << endl;
	cout << "press j -> up side move" << endl;
	cout << "press k -> down side move" << endl;

	cout << endl;
	cout << "press q or ESCAPE : quit example" << endl;
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
			//선택된 점의 위로 향하는 벡터
			vec3 v1 = p[(36 - j) % 36][(18 - i - 1) % 18] - p[(36 - j) % 36][(18 - i) % 18];
			//선택된 점의 오른쪽으로 향하는 벡터
			vec3 v2 = p[(36 - j - 1) % 36][(18 - i) % 18] - p[(36 - j) % 36][(18 - i) % 18];
			np[j][i] = cross(v1, v2);

			np[j][i] = normalize(np[j][i]);
		}
	}

	//calculate vector of vertex-to-eye
	for (int j = 0; j < 36; j++) {
		for (int i = 0; i < 18; i++) {
			vec3 v = vec3((p[(36 - j) % 36][(18 - i) % 18].x +
				p[(36 - j - 1) % 36][(18 - i) % 18].x +
				p[(36 - j - 1) % 36][(18 - i - 1) % 18].x +
				p[(36 - j) % 36][(18 - i - 1) % 18].x) / 4,
				(p[(36 - j) % 36][(18 - i) % 18].y +
					p[(36 - j - 1) % 36][(18 - i) % 18].y +
					p[(36 - j - 1) % 36][(18 - i - 1) % 18].y +
					p[(36 - j) % 36][(18 - i - 1) % 18].y) / 4,
					(p[(36 - j) % 36][(18 - i) % 18].z +
						p[(36 - j - 1) % 36][(18 - i) % 18].z +
						p[(36 - j - 1) % 36][(18 - i - 1) % 18].z +
						p[(36 - j) % 36][(18 - i - 1) % 18].z) / 4);

			cp[j][i].x = eye.x - v.x;
			cp[j][i].y = eye.y - v.y;
			cp[j][i].z = eye.z - v.z;

			cp[j][i] = normalize(cp[j][i]);
		}
	}
}

void drawCircle() {
	if (isNormal)
		drawNormal();

	glPointSize(2.5f);

	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);
	for (int j = 0; j < ySweep; j++) {
		for (int i = 0; i < zSweep; i++) {
			glVertex3f(p[(36 - j) % 36][(18 - i) % 18].x, p[(36 - j) % 36][(18 - i) % 18].y, p[(36 - j) % 36][(18 - i) % 18].z);
		}
	}
	glEnd();
	bfselection = 1;
}

void wireFrame() {
	if (isNormal)
		drawNormal();

	glPolygonOffset(0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 0, 0);

	glBegin(GL_QUADS);
	for (int j = 0; j < ySweep; j++) {
		for (int i = 0; i < zSweep; i++) {
			// 선택점 - 선택 오른쪽-  선택 오른쪽 상단 - 선택 상단 순서 --- 이하 동일
			glVertex3f(p[(36 - j) % 36][(18 - i) % 18].x,
				p[(36 - j) % 36][(18 - i) % 18].y,
				p[(36 - j) % 36][(18 - i) % 18].z);
			glVertex3f(p[(36 - j - 1) % 36][(18 - i) % 18].x,
				p[(36 - j - 1) % 36][(18 - i) % 18].y,
				p[(36 - j - 1) % 36][(18 - i) % 18].z);
			glVertex3f(p[(36 - j - 1) % 36][(18 - i - 1) % 18].x,
				p[(36 - j - 1) % 36][(18 - i - 1) % 18].y,
				p[(36 - j - 1) % 36][(18 - i - 1) % 18].z);
			glVertex3f(p[(36 - j) % 36][(18 - i - 1) % 18].x,
				p[(36 - j) % 36][(18 - i - 1) % 18].y,
				p[(36 - j) % 36][(18 - i - 1) % 18].z);
		}
	}
	glEnd();
	bfselection = 2;
}

void drawQuads() {
	if (isNormal)
		drawNormal();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(0.5, 1);

	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	for (int j = 0; j < ySweep; j++) {
		for (int i = 0; i < zSweep; i++) {
			glVertex3f(p[(36 - j) % 36][(18 - i) % 18].x, p[(36 - j) % 36][(18 - i) % 18].y, p[(36 - j) % 36][(18 - i) % 18].z);
			glVertex3f(p[(36 - j - 1) % 36][(18 - i) % 18].x, p[(36 - j - 1) % 36][(18 - i) % 18].y, p[(36 - j - 1) % 36][(18 - i) % 18].z);
			glVertex3f(p[(36 - j - 1) % 36][(18 - i - 1) % 18].x, p[(36 - j - 1) % 36][(18 - i - 1) % 18].y, p[(36 - j - 1) % 36][(18 - i - 1) % 18].z);
			glVertex3f(p[(36 - j) % 36][(18 - i - 1) % 18].x, p[(36 - j) % 36][(18 - i - 1) % 18].y, p[(36 - j) % 36][(18 - i - 1) % 18].z);
		}
	}
	glEnd();
	bfselection = 3;
}

void drawShadingQuads() {
	wireFrame();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(0.5, 1);
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	for (int j = 0; j < ySweep; j++) {
		for (int i = 0; i < zSweep; i++) {
			if (dot(np[j][i], cp[j][i]) > 0)
				glColor3f(0, 0, 1);
			else
				glColor3f(1, 0, 0);
			glVertex3f(p[(36 - j) % 36][(18 - i) % 18].x, p[(36 - j) % 36][(18 - i) % 18].y, p[(36 - j) % 36][(18 - i) % 18].z);
			glVertex3f(p[(36 - j - 1) % 36][(18 - i) % 18].x, p[(36 - j - 1) % 36][(18 - i) % 18].y, p[(36 - j - 1) % 36][(18 - i) % 18].z);
			glVertex3f(p[(36 - j - 1) % 36][(18 - i - 1) % 18].x, p[(36 - j - 1) % 36][(18 - i - 1) % 18].y, p[(36 - j - 1) % 36][(18 - i - 1) % 18].z);
			glVertex3f(p[(36 - j) % 36][(18 - i - 1) % 18].x, p[(36 - j) % 36][(18 - i - 1) % 18].y, p[(36 - j) % 36][(18 - i - 1) % 18].z);
		}
	}
	glEnd();
	bfselection = 6;
}

void drawFrameAndQuads() {
	wireFrame();
	drawQuads();
	bfselection = 4;
}

void drawNormal() {
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);
	for (int j = 0; j < ySweep; j++) {
		for (int i = 0; i < zSweep; i++) {
			vec3 v = vec3((p[(36 - j) % 36][(18 - i) % 18].x +
				p[(36 - j - 1) % 36][(18 - i) % 18].x +
				p[(36 - j - 1) % 36][(18 - i - 1) % 18].x +
				p[(36 - j) % 36][(18 - i - 1) % 18].x) / 4,
				(p[(36 - j) % 36][(18 - i) % 18].y +
					p[(36 - j - 1) % 36][(18 - i) % 18].y +
					p[(36 - j - 1) % 36][(18 - i - 1) % 18].y +
					p[(36 - j) % 36][(18 - i - 1) % 18].y) / 4,
					(p[(36 - j) % 36][(18 - i) % 18].z +
						p[(36 - j - 1) % 36][(18 - i) % 18].z +
						p[(36 - j - 1) % 36][(18 - i - 1) % 18].z +
						p[(36 - j) % 36][(18 - i - 1) % 18].z) / 4);
			glVertex3f(v.x, v.y, v.z);
			glVertex3f(np[j][i].x / 12 + v.x, np[j][i].y / 12 + v.y, np[j][i].z / 12 + v.z);
		}
	}
	glEnd();
}

void drawNormalwithOther() {
	if (isNormal) {
		if (bfselection == 1) {
			drawCircle();
			drawNormal();
		}
		else if (bfselection == 2) {
			wireFrame();
			drawNormal();
		}
		else if (bfselection == 3) {
			drawQuads();
			drawNormal();
		}
		else if (bfselection == 4) {
			drawFrameAndQuads();
			drawNormal();
		}
		else if (bfselection == 6) {
			drawShadingQuads();
			drawNormal();
		}
	}
	else
	{
		if (bfselection == 1) {
			drawCircle();
		}
		else if (bfselection == 2) {
			wireFrame();
		}
		else if (bfselection == 3) {
			drawQuads();
		}
		else if (bfselection == 4) {
			drawFrameAndQuads();
		}
		else if (bfselection == 6) {
			drawShadingQuads();
		}
	}

}

void yAxisSweepRight() {
	ySweep = glm::min(ySweep + 1, 36);
}

void yAxisSweepLeft() {
	ySweep = glm::max(ySweep - 1, 1);
}

void zAxisSweepUp() {
	zSweep = glm::min(zSweep + 1, 18);
}

void zAxisSweepDown() {
	zSweep = glm::max(zSweep - 1, 1);
}

void render(GLFWwindow* window) {
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(5 * dpiScaling);

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	switch (selection) {
	case 1: drawCircle(); break;
	case 2: wireFrame(); break;
	case 3: drawQuads(); break;
	case 4: drawFrameAndQuads(); break;
	case 5: drawNormalwithOther(); break;
	case 6: drawShadingQuads(); break;
	}
}

void keyboard(GLFWwindow* window, int key, int code, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
			// drawing torus
		case GLFW_KEY_1: selection = 1; break;
		case GLFW_KEY_2: selection = 2; break;
		case GLFW_KEY_3: selection = 3; break;
		case GLFW_KEY_4: selection = 4; break;
		case GLFW_KEY_5: isNormal = !isNormal; selection = 5; break;
		case GLFW_KEY_6: selection = 6; break;
			//Sweep torus
		case GLFW_KEY_A: yAxisSweepLeft(); break;
		case GLFW_KEY_S: yAxisSweepRight(); break;
		case GLFW_KEY_J: zAxisSweepUp(); break;
		case GLFW_KEY_K: zAxisSweepDown(); break;
		}
	}
}