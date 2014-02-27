#include "BoxRigidbody.h"

BoxRigidbody::BoxRigidbody (GameObject *gameObject, const btVector3& boxHalfExtents, float mass, 
														int col_mask, int col_to_masks) : Rigidbody(gameObject) 
{
	collisionShape = new btBoxShape (boxHalfExtents);

	motionState = new btDefaultMotionState(btTransform(btQuaternion(_transform->rotX, _transform->rotY, _transform->rotZ, _transform->rotW)
		, btVector3(_transform->posX, _transform->posY, _transform->posZ)));
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,                  // mass
		motionState,        // initial position
		collisionShape,              // collision shape of body
		btVector3(0,0,0)    // local inertia
	);

	rigidBody = new btRigidBody(rigidBodyCI);
	dynamics_world->addRigidBody(rigidBody, col_mask, col_to_masks);

	rigidBody->setUserPointer(gameObject);

	if (rigidBody->isStaticOrKinematicObject())
		Rigidbody::updateRigidbodyFromTransform();
	else
		Rigidbody::updateTransformFromRigidbody();
}

BoxRigidbody::~BoxRigidbody()
{
	dynamics_world->removeRigidBody(rigidBody);
	delete collisionShape;
	delete motionState;
	delete rigidBody;
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

