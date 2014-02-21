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
		btDiscreteDynamicsWorld* dynamicsWorld;

	public:
		Rigidbody(GameObject*);
		~Rigidbody();

		virtual void update();
		virtual void updateRigidbodyFromTransform();
		virtual void updateTransformFromRigidbody();
		virtual bool needsCollision(btBroadphaseProxy*) const;

		virtual btScalar addSingleResult(btManifoldPoint& cp,
		    const btCollisionObjectWrapper*, int, int,
		    const btCollisionObjectWrapper*, int, int);

		virtual void OnCollision (btVector3*, btVector3*, GameObject*);

		Transform* _transform;
		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidBody;
};

#endif // #ifndef __Rigidbody_h_