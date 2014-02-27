#include "Rigidbody.h"

Rigidbody::Rigidbody(GameObject *gameObject) : Component(gameObject) 
{
	onCollision = NULL;

	_transform = gameObject->getComponent<Transform>();

	dynamics_world = _gameObject->scene->physics_world;
}

Rigidbody::~Rigidbody() 
{
}

void Rigidbody::update() 
{
	Component::update();

	if (rigidBody->isStaticOrKinematicObject())
		updateRigidbodyFromTransform();
	else
		updateTransformFromRigidbody();
	
	if(onCollision != NULL)
		dynamics_world->contactTest(rigidBody, *((btCollisionWorld::ContactResultCallback*)this));
}

void Rigidbody::updateRigidbodyFromTransform () 
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3 (_transform->posX, _transform->posY, _transform->posZ));
	trans.setRotation(btQuaternion(_transform->rotX, _transform->rotY, _transform->rotZ, _transform->rotW));
	rigidBody->getMotionState()->setWorldTransform(trans);
}

void Rigidbody::updateTransformFromRigidbody () 
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	_transform->posX = trans.getOrigin().getX();
	_transform->posY = trans.getOrigin().getY();
	_transform->posZ = trans.getOrigin().getZ();

	btQuaternion rigQuat = trans.getRotation();
	_transform->rotX = rigQuat.getAxis().getX();
	_transform->rotY = rigQuat.getAxis().getY();
	_transform->rotZ = rigQuat.getAxis().getZ();
	_transform->rotW = rigQuat.getAngle();
}

//! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
/*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
 *  then this is unnecessary—checkCollideWithOverride isn't available */
bool Rigidbody::needsCollision(btBroadphaseProxy* proxy) const 
{
	// superclass will check m_collisionFilterGroup and m_collisionFilterMask
	if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
		return false;
	// if passed filters, may also want to avoid contacts between constraints
	return rigidBody->checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
}

//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
btScalar Rigidbody::addSingleResult(btManifoldPoint& cp,
	const btCollisionObject* colObj0,int partId0,int index0,
	const btCollisionObject* colObj1,int partId1,int index1)
{
	GameObject* gameObject;
	if(colObj0 == rigidBody) {
		gameObject = (GameObject*)(colObj1->getUserPointer());
	} else {
		assert(colObj1==rigidBody && "body does not match either collision object");
		gameObject = (GameObject*)(colObj0->getUserPointer());
	}

	onCollision((cp.m_positionWorldOnB), (cp.m_normalWorldOnB), gameObject);
	
	return 0; 
}

