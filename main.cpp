/////////////////////////////////////////////////////////////////////////////////////////        
// Description:
// This program simply draws the number 8 with two triangle strips, but either user can 
// change the angle between two different circles so up one will move up or down
//
// Interaction:
// Press the space bar to toggle between wirefrime and filled, +/- to increment or decrement angle respectively
///////////////////////////////////////////////////////////////////////////////////////// 

#define _USE_MATH_DEFINES

#include "Draw8Number.h"

// Drawing routine.
void drawScene(void)
{
	if (Draw8Number::isWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Draw8Number::get().draw();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	Draw8Number::get();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
	case '-':
	{
		auto& machine = Draw8Number::get();
		auto angle = machine.getDownAngle();

		angle += (key == '+' ? 1 : -1);

		machine.setDownAngle(angle);

		glutPostRedisplay();
		break;
	}
	case ' ':
		Draw8Number::isWire = !Draw8Number::isWire;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Use + or - to increment or decrement touch angle of bottom circle" << std::endl;
}

// Main routine.
int main(int argc, char** argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Draw8Number");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
