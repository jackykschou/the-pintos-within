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