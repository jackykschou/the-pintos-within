#include "PhysicsManager.h"

void PhysicsManager::initialize() 
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
}

btDiscreteDynamicsWorld *PhysicsManager::createDynamicsWorld() 
{
	btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	btOverlapFilterCallback * filterCallback = new FilterCallback();
   	dynamicsWorld->getPairCache()->setOverlapFilterCallback(filterCallback);
	return dynamicsWorld;
}

void PhysicsManager::updatePhysics(btDiscreteDynamicsWorld *dynamicsWorld, const float elapsedTime) {
	dynamicsWorld->stepSimulation(elapsedTime, 5, 0.017);
}

