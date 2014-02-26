#include "BoxRigidbody.h"

BoxRigidbody::BoxRigidbody (GameObject *gameObject, const btVector3& boxHalfExtents, float mass, 
														int col_mask, int col_to_masks) : Rigidbody(gameObject) 
{
	collisionShape = new btBoxShape(boxHalfExtents);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),btVector3(0, 50, 0)));
    btVector3 inertia(0,0,0);
    collisionShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, inertia);
    rigidBody = new btRigidBody(rigidBodyCI);
    dynamics_world->addRigidBody(rigidBody, col_mask, col_to_masks);

	rigidBody->setUserPointer(gameObject);
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

