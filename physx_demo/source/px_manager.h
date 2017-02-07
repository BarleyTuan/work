#ifndef __PX_MANAGER_H__
#define __PX_MANAGER_H__

#include <vector>
#include <map> 
#include <iostream>

#include <PxPhysicsAPI.h> 
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h> 
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxShapeExt.h>
#include <foundation/PxMat33.h> 
#include <extensions/PxSimpleFactory.h>
#include <characterkinematic/PxController.h>
#include <characterkinematic/PxControllerManager.h>
#include <boost/python.hpp>

using namespace physx;
using namespace std;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;

class PxManager {
public:
	PxManager() {
		gPhysicsSDK = NULL;
		gFoundation = NULL;
		manager = NULL;
		gScene = NULL;
	}
	~PxManager() {
		ShutdownPhysX();
		character_controller_map.clear();
		boxes.clear();
	}
	void ShutdownPhysX();
	void InitializePhysX();
	void UpdatePhysX(float myTimestep);
	void CreateGroundPlane();
	void CreateCube(PxVec3 pos);
	void AddController(int actor_id, PxExtendedVec3 init_pos);
	void AddController2(int actor_id, float x, float y, float z);
	
	PxExtendedVec3 GetPosition(int actor_id);
	boost::python::list GetPosition2(int actor_id);

	void MovePosition(int actor_id, PxVec3 diff_pos, float diff_time);
	void MovePosition2(int actor_id, float x, float y, float z, float diff_time) {
		MovePosition(actor_id, PxVec3(x, y, z), diff_time);
	}

	std::map<int, PxController *> character_controller_map;
	std::vector<PxRigidActor*> boxes;
	PxScene* gScene;
	PxPhysics* gPhysicsSDK;
	PxFoundation* gFoundation;
	PxControllerManager* manager;
};

#endif // __PX_MANAGER_H__