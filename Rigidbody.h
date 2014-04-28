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
		btRigidBody::btRigidBodyConstructionInfo* _rigid_info;

	public:
		float xOffset;
		float yOffset;
		float zOffset;

		Rigidbody(GameObject*, float, float, float);
		virtual ~Rigidbody();

		virtual void update();
		virtual void updateRigidbodyFromTransform();
		virtual void updateTransformFromRigidbody();
		virtual bool needsCollision(btBroadphaseProxy*) const;

		virtual btScalar addSingleResult(btManifoldPoint& cp,
		    const btCollisionObject*, int, int,
		    const btCollisionObject*, int, int);

		void (*onCollision)(btVector3, btVector3, GameObject*, GameObject*);

		btDiscreteDynamicsWorld* dynamics_world;
		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidbody;
};

#endif // #ifndef __Rigidbody_h_