#include "SphereRigidbody.h"

SphereRigidbody::SphereRigidbody (GameObject *gameObject, float radius, float mass) : Rigidbody(gameObject) 
{

	collisionShape = new btSphereShape(radius);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),btVector3(0, 50, 0)));
    btVector3 inertia(0,0,0);
    collisionShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, inertia);
    rigidBody = new btRigidBody(rigidBodyCI);
    dynamics_world->addRigidBody(rigidBody);

	rigidBody->setUserPointer(gameObject);
}

SphereRigidbody::~SphereRigidbody()
{
	dynamics_world->removeRigidBody(rigidBody);
	delete collisionShape;
	delete motionState;
	delete rigidBody;
}

void SphereRigidbody::update() 
{
	Rigidbody::update();
}