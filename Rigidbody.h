#ifndef __Rigidbody_h_
#define __Rigidbody_h_

#include "common.h"

class Rigidbody : Component {
	protected:
		btDiscreteDynamicsWorld* dynamicsWorld;
	public:
		void Rigidbody(GameObject gameObject);
		void update();
		void ~Rigidbody();
		Transform* _transform;
		btCollisionShape* collisionShape;
		btDefaultMotionState* motionState;
		btRigidBody* rigidBody;
		bool kinematic = false;

};

#endif // #ifndef __Rigidbody_h_