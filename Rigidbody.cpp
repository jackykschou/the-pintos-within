void Rigidbody::Rigidbody(GameObject gameObject) : Component(gameObject) {

	if((_transform = gameObject.GetComponent<Transform>()) == NULL) {
		_transform = GameObject.addComponent(new Transform());
	}
	dynamicsWorld = _gameObject->scene->physicsWorld;

void Rigidbody::update() {
	if (kinematic)
		UpdateRigidbodyFromTransform();
	else
		UpdateTransformFromRigidbody();
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