#ifndef __GL_MANAGER_H__
#define __GL_MANAGER_H__

#include <iostream>
#include <GL/freeglut.h>
#include "px_manager.h"
#include <Python.h>  

using namespace std;
using namespace physx;

const int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;

class GlManager {
public:
	~GlManager() {
	    px_manager = NULL;
		p_module = NULL;
		update_func = NULL;
		key_func = NULL;

	}
	static GlManager* getInstance() {
		if (gl_manager_ == NULL) {
			gl_manager_ = new GlManager();
		};
		return gl_manager_;
	}
	void SetPxManager(PxManager* px) {
	  px_manager = px;
	}
	void InitPyModule() {
		Py_Initialize();
		p_module =PyImport_ImportModule("game_world");
		update_func = PyObject_GetAttrString(p_module, "update");
		key_func = PyObject_GetAttrString(p_module, "on_key_down");
	}

    void DrawGrid(int grid_size);
	void getColumnMajor(PxMat33 m, PxVec3 t, float* mat);
	void DrawBox(PxShape* pShape, PxRigidActor* actor);
	void DrawShape(PxShape* shape, PxRigidActor* actor);
	void DrawActor(PxRigidActor* actor);
	void RenderActors();
	void OnRender();
	void OnKeyboard(unsigned char key, int mx, int my);
	void OnKeyboardUp(unsigned char key, int mx, int my);
private:
	PxManager *px_manager;
	float add_x, add_y, add_z;
	int pre_tick;
	PyObject* p_module;
	PyObject* update_func;
	PyObject* key_func;

	GlManager() {
		px_manager = NULL;
		add_x = add_y = add_z = 0;
		pre_tick = glutGet(GLUT_ELAPSED_TIME);
		p_module = NULL;
		update_func = NULL;
		key_func = NULL;
	}
    static GlManager* gl_manager_;
};

#endif // __GL_MANAGER_H__