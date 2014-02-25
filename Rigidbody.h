#ifndef __Rigidbody_h_
#define __Rigidbody_h_

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
		virtual ~Rigidbody() = 0;

		virtual void update();
		virtual void updateRigidbodyFromTransform();
		virtual void updateTransformFromRigidbody();
		virtual bool needsCollision(btBroadphaseProxy*) const;

		virtual btScalar addSingleResult(btManifoldPoint& cp,
		    const btCollisionObjectWrapper*, int, int,
		    const btCollisionObjectWrapper*, int, int);

		void (*onCollision)(btVector3, btVector3, GameObject*);

		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidBody;
};

#endif // #ifndef __Rigidbody_h_