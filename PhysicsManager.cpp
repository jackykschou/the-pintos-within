#include "Singleton.h"

#include "PhysicsManager.h"
#include <btBulletDynamicsCommon.h>
void PhysicsManager::initialize() {

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	// btAlignedObjectArray<btCollisionShape*> collisionShapes;

}

btDiscreteDynamicsWorld *PhysicsManager::createDynamicsWorld() {
	btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -0.098, 0));
	return dynamicsWorld;
}

void PhysicsManager::updatePhysics (btDiscreteDynamicsWorld *dynamicsWorld, const float elapsedTime) {
	dynamicsWorld->stepSimulation(elapsedTime, 5, 0.017);
}

int main(){}