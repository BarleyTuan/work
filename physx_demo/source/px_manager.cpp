#include "px_manager.h"

void PxManager::ShutdownPhysX() {
	for(int i = 0; i<boxes.size(); i++) {
		gScene->removeActor(*boxes[i]);
	}
	gScene->release();
	for(vector<PxRigidActor*>::iterator i = boxes.begin(); i<boxes.end(); i++) {
		(*i)->release();
	}
	manager->purgeControllers();
	gPhysicsSDK->release();
}

void PxManager::UpdatePhysX(float myTimestep) {
	if (gScene == NULL) {
		return;
	}
	gScene->simulate(myTimestep);
	//cout << "  time " << myTimestep << "   " << gScene->getGravity().y  << endl;
	
	int num = character_controller_map.size();
	for (int i = 1; i<=num; ++i) {
		float add_y = gScene->getGravity().y *  myTimestep;
		MovePosition(i, PxVec3(0, add_y, 0), 0.0f);
	}
}


void PxManager::InitializePhysX() {
	gFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	// Creating instance of PhysX SDK
	gPhysicsSDK = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*gFoundation,
		PxTolerancesScale());

	if(gPhysicsSDK==NULL) {
		cerr<<"Error creating PhysX3 device."<<endl;
		cerr<<"Exiting..."<< endl;
		exit(1);
	}

	if(!PxInitExtensions(*gPhysicsSDK))
		cerr<<"PxInitExtensions failed!"<< endl;

	//PxExtensionVisualDebugger::connect(gPhysicsSDK->getPvdConnectionManager(),"localhost",5425, 10000, true);
	//Create the scene
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);

	if(!sceneDesc.cpuDispatcher) {
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if(!mCpuDispatcher)
			cerr<<"PxDefaultCpuDispatcherCreate failed!"<< endl;
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}
	if(!sceneDesc.filterShader)
		sceneDesc.filterShader = gDefaultFilterShader;


	gScene = gPhysicsSDK->createScene(sceneDesc);
	if(!gScene)
		cerr<<"createScene failed!"<< endl;

	gScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0);
	gScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

	//==================
	CreateGroundPlane();
	int pos_list[4][3] = {
		{-4, 1, 0},{-2, 1, 0},{0, 1, 0},{2, 1, 0}
	};
	for (int i = 0; i<= 3; ++i) {
		PxVec3 pos = PxVec3(pos_list[i][0], pos_list[i][1], pos_list[i][2]);
		CreateCube(pos);
	}
	manager = PxCreateControllerManager(*gScene);
	//AddController(1, PxExtendedVec3(1.0, 1.0, 4.0));
	//AddController(2, PxExtendedVec3(-2.0, 1.0, 4.0));
}

void PxManager::CreateGroundPlane() {
	//1) Create ground plane
	PxReal d = 0.0f;
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	PxRigidStatic* plane = gPhysicsSDK->createRigidStatic(pose);
	if(!plane)
		cerr<<"create plane failed!"<< endl;
	
	PxMaterial* mMaterial = gPhysicsSDK->createMaterial(0.1, 0.2, 0.5);
	PxShape* shape = plane->createShape(PxPlaneGeometry(), *mMaterial);
	if(!shape)
		cerr<<"create shape failed!"<< endl;
	gScene->addActor(*plane);
}

void PxManager::CreateCube(PxVec3 pos) { 
	PxReal density = 1.0f;
	PxTransform transform(pos, PxQuat::createIdentity());
	PxVec3 dimensions(1, 1, 1);
	PxBoxGeometry geometry(dimensions);

	PxMaterial* mMaterial = gPhysicsSDK->createMaterial(0.1, 0.2, 0.5);
	PxRigidDynamic *actor = PxCreateDynamic(*gPhysicsSDK, transform, geometry, *mMaterial, density);
	actor->setAngularDamping(0.75);
	actor->setLinearVelocity(PxVec3(0, 0, 0));
	if(!actor)
		cerr<<"create actor failed!"<<endl;
	gScene->addActor(*actor);
	boxes.push_back(actor);
}

void PxManager::AddController2(int actor_id, float x, float y, float z) {
	cout << "add controller " << actor_id << " pos " << x << " " << y << " " << z << endl;
	PxExtendedVec3 init_pos = PxExtendedVec3(x, y, z);
	AddController(actor_id, init_pos);
}

void PxManager::AddController(int actor_id, PxExtendedVec3 init_pos) {
	// Code for character controller
	if(!manager) {
		cout<<"Unable to create character controller manager!"<<endl;
	}

	PxMaterial* capsuleMaterial = gPhysicsSDK->createMaterial(0.4f, 0.2f, 0.1f);
	PxCapsuleControllerDesc capsuleDesc;
	capsuleDesc.position = init_pos;
	capsuleDesc.contactOffset = 0.05f;
	capsuleDesc.stepOffset = 0.01f;
	capsuleDesc.slopeLimit = 0.5f;
	capsuleDesc.radius = 0.5f;
	capsuleDesc.height = 0.5f;
	capsuleDesc.upDirection = PxVec3(0, 1, 0);
	capsuleDesc.material = capsuleMaterial;

	PxController *characterController = manager->createController(capsuleDesc);
	if(characterController) {
		PxRigidDynamic* actor = characterController->getActor();
		actor->setMass(PxReal(10.0f));
		// Do something
	} else {
		cerr<<"Unable to create character controller"<<endl;
	}
	character_controller_map[actor_id] = characterController;
}

PxExtendedVec3 PxManager::GetPosition(int actor_id) {
	PxExtendedVec3 pos = character_controller_map[actor_id]->getPosition();
    return pos;
}

boost::python::list PxManager::GetPosition2(int actor_id) { 
	boost::python::list ret;
	if (character_controller_map.find(actor_id) == character_controller_map.end()) {
		ret.append(0);
		ret.append(0);
		ret.append(0);
		return ret;
	}
	PxExtendedVec3 pos = character_controller_map[actor_id]->getPosition();
	ret.append(pos.x);
	ret.append(pos.y);
	ret.append(pos.z);
	return ret;
}

void PxManager::MovePosition(int actor_id, PxVec3 diff_pos, float diff_time) {
	if (character_controller_map.find(actor_id) == character_controller_map.end()) {
		return;
	}
	PxControllerCollisionFlags collisionFlags =
		character_controller_map[actor_id]->move(diff_pos, 0.0f, 0.0f, NULL);
}