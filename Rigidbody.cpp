void Rigidbody::Rigidbody(GameObject *gameObject) : Component(gameObject) {

	if((_transform = gameObject.GetComponent<Transform>()) == NULL) {
		_transform = GameObject.addComponent(new Transform());
	}
	dynamicsWorld = _gameObject->scene->physicsWorld;

void Rigidbody::update() {
	if (rigidBody.isStaticOrKinematic())
		UpdateTransformFromRigidbody();
	else
		UpdateRigidbodyFromTransform();
}

void Rigidbody::UpdateRigidbodyFromTransform () {
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	//should these constructors instead be pointers?
	trans.setOrigin(btVector3 (_transform.posX, _transform.posY, _transform.posZ));
	trans.setRotation(btQuaternion(_transform.rotX, _transform.rotY, _transform.rotZ, _transform.rotW))
	rigidBody->getMotionState()->setWorldTransform(trans);
}
void Rigidbody::UpdateTransformFromRigidbody () {
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);
	_transform.posX = trans.getOrigin().getX();
	_transform.posY = trans.getOrigin().getY();
	_transform.posZ = trans.getOrigin().getZ();

	btQuaternion rigQuat = trans.getRotation();
	_transform.rotX = rigQuat.getAxis().getX();
	_transform.rotY = rigQuat.getAxis().getY();
	_transform.rotZ = rigQuat.getAxis().getZ();
	_transform.rotW = rigQuat.getAngle();
}

void Rigidbody::~Rigidbody() {
	dynamicsWorld->removeRigidBody(rigidBody);
	delete collisionShape;
	delete motionState;
	delete rigidBody;
}


	//! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
	/*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
	 *  then this is unnecessary—checkCollideWithOverride isn't available */
	bool Rigidbody::needsCollision(btBroadphaseProxy* proxy) const {
		// superclass will check m_collisionFilterGroup and m_collisionFilterMask
		if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
			return false;
		// if passed filters, may also want to avoid contacts between constraints
		return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
	}

	//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
	btScalar Rigidbody::addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0,int partId0,int index0,
		const btCollisionObjectWrapper* colObj1,int partId1,int index1)
	{
		btVector3 pt; // will be set to point of collision relative to body
		if(colObj0->m_collisionObject==&body) {
			pt = cp.m_localPointA;
		} else {
			assert(colObj1->m_collisionObject==&body && "body does not match either collision object");
			pt = cp.m_localPointB;
		}
		// do stuff with the collision point
		return 0; // not actually sure if return value is used for anything...?
	}