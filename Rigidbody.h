#ifndef __Rigidbody_h_
#define __Rigidbody_h_

#include "common.h"

class Rigidbody : public Component, public ContactResultCallback {
	protected:
		virtual btDiscreteDynamicsWorld* dynamicsWorld;
	public:
		virtual void Rigidbody(GameObject *gameObject);
		virtual void update();
		virtual void ~Rigidbody();
		virtual void UpdateRigidbodyFromTransform ();
		virtual void UpdateTransformFromRigidbody ();
		virtual bool needsCollision(btBroadphaseProxy* proxy);
		virtual btScalar addSingleResult(btManifoldPoint& cp,
		    const btCollisionObjectWrapper* colObj0,int partId0,int index0,
		    const btCollisionObjectWrapper* colObj1,int partId1,int index1);
		virtual void OnCollision (); //TODO: FINISH

		Transform* _transform;
		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidBody;
		bool kinematic = false;

};

#endif // #ifndef __Rigidbody_h_