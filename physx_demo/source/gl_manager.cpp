#include "gl_manager.h"

void GlManager::DrawGrid(int grid_size) {
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for(int i = -grid_size; i<=grid_size; i++) {
		glVertex3f((float) i, 0, (float) -grid_size);
		glVertex3f((float) i, 0, (float) grid_size);

		glVertex3f((float) -grid_size, 0, (float) i);
		glVertex3f((float) grid_size, 0, (float) i);
	}
	glEnd();
}
void GlManager::getColumnMajor(PxMat33 m, PxVec3 t, float* mat) {
	mat[0] = m.column0[0];
	mat[1] = m.column0[1];
	mat[2] = m.column0[2];
	mat[3] = 0;

	mat[4] = m.column1[0];
	mat[5] = m.column1[1];
	mat[6] = m.column1[2];
	mat[7] = 0;

	mat[8] = m.column2[0];
	mat[9] = m.column2[1];
	mat[10] = m.column2[2];
	mat[11] = 0;

	mat[12] = t[0];
	mat[13] = t[1];
	mat[14] = t[2];
	mat[15] = 1;
}
void GlManager::DrawBox(PxShape* pShape, PxRigidActor* actor) {
	PxTransform pT = PxShapeExt::getGlobalPose(*pShape, *actor);
	PxBoxGeometry bg;
	pShape->getBoxGeometry(bg);
	PxMat33 m = PxMat33(pT.q);
	float mat[16];
	getColumnMajor(m, pT.p, mat);
	glPushMatrix();
	glMultMatrixf(mat);
	glutSolidCube(bg.halfExtents.x*2);
	glPopMatrix();
}
void GlManager::DrawShape(PxShape* shape, PxRigidActor* actor) {
	PxGeometryType::Enum type = shape->getGeometryType();
	switch(type) {
	case PxGeometryType::eBOX:
		DrawBox(shape, actor);
		break;
	}
}
void GlManager::DrawActor(PxRigidActor* actor) {
	PxU32 nShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];

	actor->getShapes(shapes, nShapes);
	while(nShapes--) {
		DrawShape(shapes[nShapes], actor);
	}
	delete[] shapes;
}

void GlManager::RenderActors() {
	GLfloat mat_diffuse[4] = {0.85f, 0, 0, 0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	for (int i = 0; i < px_manager->boxes.size(); ++i) {
		DrawActor(px_manager->boxes[i]);
	}
	for(map<int, PxController *>::iterator iter = px_manager->character_controller_map.begin();
		iter != px_manager->character_controller_map.end(); ++iter) {
			PxController *controller = iter->second;
			glPushMatrix();
			GLfloat mat_diffuse2[4] = {0.0f, 1.0f, 0, 0};
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse2);
			const PxExtendedVec3 position = controller->getPosition();
			glTranslatef(
				controller->getPosition().x,
				controller->getPosition().y,
				controller->getPosition().z);
			glutSolidCube(1.0f);
			glPopMatrix();
	}
}

void GlManager::OnRender() {
	int current = glutGet(GLUT_ELAPSED_TIME);
	float diff_time = (current - pre_tick) / 1000.0;
	
	if (px_manager->gScene) {
		px_manager->UpdatePhysX(diff_time);
	}
	//Draw the grid and axes

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -10);
	glRotatef(15, 1, 0, 0);
	glRotatef(0, 0, 1, 0);

	DrawGrid(10);
	glEnable(GL_LIGHTING);
	RenderActors();
	glDisable(GL_LIGHTING);

	glColor3f(1, 1, 1);
	glutSwapBuffers();
	  
	PyEval_CallObject(update_func, Py_BuildValue("(i)", current-pre_tick));
	pre_tick = current;
}

void GlManager::OnKeyboard(unsigned char key, int mx, int my) {
	PyEval_CallObject(key_func, Py_BuildValue("(c)", key));
	/*
	float movement = 1;
	x = add_y = add_z = 0;
	if(key=='j') {
		add_y = 3 * movement;
	}
	if(key=='w') {
		add_z = -movement;
	}
	if(key=='s') {
		add_z = movement;
	}
	if(key=='d') {
		add_x = movement;
	}
	if(key=='a') {
		add_x = -movement;
	}
	px_manager->MovePosition(1, PxVec3(add_x, add_y, add_z), 1);
	add_x = add_y = add_z = 0;
	*/
}
void GlManager::OnKeyboardUp(unsigned char key, int mx, int my) {
}
