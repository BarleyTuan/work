#include <boost/python.hpp>
#include "gl_manager_wrapper.h"
#include "px_manager.h"
#include "main_logic.h"

using namespace boost::python;
using namespace std;
using namespace physx;

#ifndef linux
#include "gl_manager.h"
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PxTask.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x86")
GlManager* GlManager::gl_manager_ = NULL;
#endif

BOOST_PYTHON_MODULE(glwrapper)
{
#ifndef linux
	def("logic_main", logic_main);
#endif
	class_<PxManager>("PxManager")
        .def("InitializePhysX", &PxManager::InitializePhysX)
        .def("MovePosition", &PxManager::MovePosition2)
	    .def("GetPosition",  &PxManager::GetPosition2)
        .def("AddController", &PxManager::AddController2)
		.def("UpdatePhysX", &PxManager::UpdatePhysX)
	;
}

void InitGlWrapper() {
	initglwrapper();
}