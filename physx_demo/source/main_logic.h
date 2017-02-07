#ifndef __MAIN_LOGIC_H__
#define __MAIN_LOGIC_H__

#include "px_manager.h"

void OnIdle();
void OnReshape(int nw, int nh);
void OnRender();
void OnKeyboard(unsigned char key, int mx, int my) ;
void OnKeyboardUp(unsigned char key, int mx, int my) ;
void InitGL() ;
void logic_main(PxManager *px_manager);

#endif // __MAIN_LOGIC_H__