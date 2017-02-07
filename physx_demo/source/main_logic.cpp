#include <iostream>
#include "main_logic.h"
#include "px_manager.h"
#include "gl_manager.h"

void OnIdle() { 
	glutPostRedisplay(); 
}

void OnReshape(int nw, int nh) {
	glViewport(0, 0, nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat) nw/(GLfloat) nh, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void OnRender() {
   GlManager::getInstance()->OnRender();
}
void OnKeyboard(unsigned char key, int mx, int my) {
   GlManager::getInstance()->OnKeyboard(key, mx, my);
}
void OnKeyboardUp(unsigned char key, int mx, int my) {
	//GlManager::getInstance()->OnKeyboardUp(key, mx, my);
}

void InitGL() {
	int ac = 1;
	char* av = "testgl";
	glutInit(&ac, &av);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("GLUT PhysX3 Demo - Simple Box");

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[4] = {0.25f, 0.25f, 0.25f, 0.25f};
	GLfloat diffuse[4] = {1, 1, 1, 1};
	GLfloat mat_diffuse[4] = {0.85f, 0, 0, 0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glDisable(GL_LIGHTING);

	// initcallback
	glutIdleFunc(OnIdle);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyboard);
	glutKeyboardUpFunc(OnKeyboardUp);
}

void logic_main(PxManager *px_manager) {
	InitGL();
	GlManager* gl_manager = GlManager::getInstance();
	gl_manager->InitPyModule();
	gl_manager->SetPxManager(px_manager);
	glutMainLoop();
}

