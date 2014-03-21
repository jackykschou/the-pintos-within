#ifndef __Trigger_h_
#define __Trigger_h_

#include "common.h"

#include "Component.h"
#include "PhysicsManager.h"
#include "Transform.h"

#include "btCollisionObjectWrapper.h"

class Transform;

class Rigidbody : public Component, public btCollisionWorld::ContactResultCallback 
{
	protected:
		Transform* _transform;
		btDiscreteDynamicsWorld* dynamics_world;

	public:
		Rigidbody(GameObject*);
		virtual ~Rigidbody();

		virtual void update();
		virtual void updateRigidbodyFromTransform();
		virtual void updateTransformFromRigidbody();
		virtual bool needsCollision(btBroadphaseProxy*) const;

		virtual btScalar addSingleResult(btManifoldPoint& cp,
		    const btCollisionObject*, int, int,
		    const btCollisionObject*, int, int);

		void (*onCollision)(btVector3, btVector3, GameObject*, GameObject*);

		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidbody;
};

#endif // #ifndef __Trigger_h_
