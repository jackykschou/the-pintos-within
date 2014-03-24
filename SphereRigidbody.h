#ifndef __SphereRigidbody_h_
#define __SphereRigidbody_h_

#include "Rigidbody.h"

class SphereRigidbody : Rigidbody
{
public:
	SphereRigidbody(GameObject*, float, float, 
					int, int, btRigidBody::btRigidBodyConstructionInfo* _rigid_info = NULL);
	virtual ~SphereRigidbody();

	virtual void update();

	virtual bool needsCollision(btBroadphaseProxy*) const;

	virtual btScalar addSingleResult(btManifoldPoint& cp,
		   	const btCollisionObject*, int, int,
		    const btCollisionObject*, int, int);

	btScalar addSingleResult(btManifoldPoint&, const btCollisionObjectWrapper*, int, int, const btCollisionObjectWrapper*, int, int) { return 0; }

};

#endif // #ifndef __SphereRigidbody_h_
