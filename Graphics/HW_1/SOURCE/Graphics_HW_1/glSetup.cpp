#include "glSetup.h"

#include <string.h>
#include <iostream>
using namespace std;

bool	perspectiveView = true;

float	screenScale = 0.5;
int		screenW = 0, screenH = 0;
int		windowW, windowH;
float	aspect;
float	dpiScaling = 0;

int		vsync = 1;

float	fovy = 46.4;
//float	fovy = 37.8;
//float	fovy = 27.0;
//float	fovy = 16.1;
//float	fovy = 11.4;
//float	fovy = 6.9;

float	nearDist = 1.0;
float	farDist = 20.0;

void	errorCallback(int error, const char* description) {
	cerr << "####" << description << endl;
}

void	setupProjectionMatrix() {
	if (perspectiveView)
		gluPerspective(fovy, aspect, nearDist, farDist);
	else
		glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -nearDist, farDist);
}

void	reshape(GLFWwindow* window, int w, int h) {
	aspect = (float)w / h;

	//set the viewport
	windowW = w;
	windowH = h;
	glViewport(0, 0, w, h);

	//initialize the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//set up a projection matrix
	setupProjectionMatrix();

	//the screen size is required for mous interaction.
	glfwGetWindowSize(window, &screenW, &screenH);
	cerr << "reshape(" << w << ", " << h << ")";
	cerr << "with screen " << screenW << " x " << screenH << endl;
}

GLFWwindow* initializeOpenGL(int argc, char* argv[], GLfloat bgColor[4]) {
	glfwSetErrorCallback(errorCallback);

	//Init GLFW
	if (!glfwInit())		exit(EXIT_FAILURE);

	//enalbe openGL 2.1 in OS x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	//create the window
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int monitorW, monitorH;
	glfwGetMonitorPhysicalSize(monitor, &monitorW, &monitorH);
	cerr << "Status: Monitor " << monitorW << "mm x " << monitorH << "mm" << endl;

	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	if (screenW == 0)	screenW = videoMode->width * screenScale;
	if (screenH == 0)	screenH = videoMode->height * screenScale;

	GLFWwindow* window = glfwCreateWindow(screenW, screenH, argv[0], NULL, NULL);
	if (!window) {
		glfwTerminate();
		cerr << "Failed in glfwCreateWindow() " << endl;
		return NULL;
	}

	//Context
	glfwMakeContextCurrent(window);

	//Clear the backGround ASAP
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glfwSwapBuffers(window);

	//Check the size of the window
	glfwGetWindowSize(window, &screenW, &screenH);
	cerr << "Status: window " << screenW << " x " << screenH << endl;

	glfwGetFramebufferSize(window, &windowW, &windowH);
	cerr << "Status: Framebuffer " << windowW << " x " << windowH << endl;

	//DPI Scaling
	if (dpiScaling == 0)	dpiScaling = float(windowW) / screenW;

	//CallBack;
	glfwSetFramebufferSizeCallback(window, reshape);

	//Get the OpenGL version and renderer;
	cout << "Status: Renderer " << glGetString(GL_RENDERER) << endl;
	cout << "Status: Ventor " << glGetString(GL_VENDOR) << endl;
	cout << "Status: OpenGL " << glGetString(GL_VERSION) << endl;

	//Vertical sync
	glfwSwapInterval(vsync);

	return window;
}

//Draw the coordinate frame with the xyz axes
void		drawAxes(float l, float w) {
	glLineWidth(w);

	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
	glEnd();
}
