#include "Singleton.h"

void PhysicsManager::initialize() {

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher();
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -0.098, 0));
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

}

void PhysicsManager::updatePhysics (const float elapsedTime, int maxSubSteps, const float fixedTimestep) {
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
}
