#ifndef __PhysicsManager_h_
#define __PhysicsManager_h_

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include "Singleton.h"

#include "common.h"

struct FilterCallback : public btOverlapFilterCallback {

protected:
   // return true when pairs need collision
   virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
   {
   	// LOG("needBroadphaseCollision");
      bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) &&
                  (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
      //add some additional logic here that modified 'collides'
      return collides;
   }
};

class PhysicsManager : public Singleton<PhysicsManager> 
{
	protected:
	public:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btConstraintSolver* mConstraintsolver;
		btCollisionWorld* mWorld;

		virtual void initialize();
		virtual void updatePhysics (btDiscreteDynamicsWorld *dynamicsWorld, const float elapsedTime);
		virtual btDiscreteDynamicsWorld *createDynamicsWorld();
};

#endif

