#include "BoxRigidbody.h"

BoxRigidbody::BoxRigidbody (GameObject *gameObject, const btVector3& boxHalfExtents, float mass, 
		int col_mask, int col_to_masks, btRigidBody::btRigidBodyConstructionInfo* _rigid_info,
		float x_offset, float y_offset, float z_offset) : Rigidbody(gameObject, x_offset, y_offset, z_offset)
{
	motionState = new btDefaultMotionState(btTransform(btQuaternion(_transform->rotX, _transform->rotY, _transform->rotZ, _transform->rotW)
		, btVector3(_transform->posX, _transform->posY, _transform->posZ)));

	if(_rigid_info == NULL)
	{
		collisionShape = new btBoxShape (boxHalfExtents);
		_rigid_info = new btRigidBody::btRigidBodyConstructionInfo(
			mass,                  // mass
			NULL,        // initial position
			collisionShape,              // collision shape of body
			btVector3(0,0,0)    // local inertia
		);
	}

	rigidbody = new btRigidBody(*_rigid_info);
	dynamics_world->addRigidBody(rigidbody, col_mask, col_to_masks);

	rigidbody->setMotionState(motionState);

	rigidbody->setUserPointer(gameObject);

	collisionShape = rigidbody->getCollisionShape();

	if (rigidbody->isStaticOrKinematicObject())
		Rigidbody::updateRigidbodyFromTransform();
	else
		Rigidbody::updateTransformFromRigidbody();

	delete _rigid_info;
}

BoxRigidbody::~BoxRigidbody()
{
	dynamics_world->removeRigidBody(rigidbody);
	delete collisionShape;
	delete motionState;
	delete rigidbody;
}

void BoxRigidbody::update() 
{
	Rigidbody::update();
}

bool BoxRigidbody::needsCollision(btBroadphaseProxy* proxy) const 
{
	Rigidbody::needsCollision(proxy);
}

//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
btScalar BoxRigidbody::addSingleResult(btManifoldPoint& cp,
	const btCollisionObject* colObj0,int partId0,int index0,
	const btCollisionObject* colObj1,int partId1,int index1)
{
	Rigidbody::addSingleResult(cp, colObj0, partId0, index0, colObj1, partId1, index1);
}

