SphereRigidbody::SphereRigidbody (GameObject *gameObject) : Rigidbody(gameObject) {

	collisionShape = new btSphereShape(1);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    collisionShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,motionState,fallShape,fallInertia);
    rigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);

	setUserPointer(gameObject);
}