#ifndef __PhysicsManager_h_
#define __PhysicsManager_h_

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "Singleton.h"

class PhysicsManager : public Singleton<PhysicsManager> {

	protected:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btConstraintSolver* mConstraintsolver;
		btCollisionWorld* mWorld;

	public:
		virtual void initialize();
		virtual void updatePhysics (btDiscreteDynamicsWorld *dynamicsWorld, const float elapsedTime);
		virtual btDiscreteDynamicsWorld *createDynamicsWorld();

};

#endif

