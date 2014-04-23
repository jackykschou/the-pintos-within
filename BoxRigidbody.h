#ifndef __BoxRigidbody_h_
#define __BoxRigidbody_h_

#include "Rigidbody.h"

class BoxRigidbody : Rigidbody
{
public:
	BoxRigidbody(GameObject*, const btVector3&, float, 
				int, int, btRigidBody::btRigidBodyConstructionInfo* _rigid_info = NULL,
				float x_offset = 0, float y_offset = 0, float z_offset = 0);
	virtual ~BoxRigidbody();

	virtual void update();

	virtual bool needsCollision(btBroadphaseProxy*) const;

	virtual btScalar addSingleResult(btManifoldPoint& cp,
		   	const btCollisionObject*, int, int,
		    const btCollisionObject*, int, int);
};

#endif // #ifndef __BoxRigidbody_h_