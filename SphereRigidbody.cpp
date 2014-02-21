#include "SphereRigidbody.h"

SphereRigidbody::SphereRigidbody (GameObject *gameObject, float mass) : Rigidbody(gameObject) 
{

	collisionShape = new btSphereShape(1);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),btVector3(0, 50, 0)));
    btVector3 inertia(0,0,0);
    collisionShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, inertia);
    rigidBody = new btRigidBody(rigidBodyCI);
    dynamicsWorld->addRigidBody(rigidBody);

	rigidBody->setUserPointer(gameObject);
}

SphereRigidbody::~SphereRigidbody()
{

}

void SphereRigidbody::update() 
{
	Rigidbody::update();
}