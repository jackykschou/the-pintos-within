#ifndef __PhysicsManager_h_
#define __PhysicsManager_h_

class PhysicsManager : Singleton<PhysicsManager> {

	protected:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		btConstraintSolver* mConstraintsolver;
		btCollisionWorld* mWorld;

	public:
		virtual void initialize();
		virtual void updatePhysics (const float elapsedTime, int maxSubSteps, const float fixedTimestep);


};

#endif
