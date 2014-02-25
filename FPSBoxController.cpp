#include "FPSBoxController.h"

FPSBoxController::FPSBoxController(GameObject* gameObject, std::string camera_name, double camera_offset, 
														const btVector3& boxHalfExtents, btScalar step_height, 
														int col_mask, int col_to_masks) : Component(gameObject)
{
	onCollision = NULL;
	can_move = true;

	base_movement_speed = 20;
	base_jump_speed = 30;

	movement_speed_multiplier = 1;
	jump_speed_multiplier = 1;

	if((_transform = gameObject->getComponent<Transform>()) == NULL)
		_transform = new Transform(gameObject);

	fps_camera = new FPSCamera(gameObject, camera_name, camera_offset);
	_collisionShape = new btBoxShape(boxHalfExtents);
	dynamics_world = _gameObject->scene->physics_world;




	_ghostObject = new btPairCachingGhostObject();
	_ghostObject->setUserPointer(gameObject);
	_ghostObject->setCollisionShape(_collisionShape);
	_ghostObject->setWorldTransform(btTransform(btQuaternion(_transform->rotX, _transform->rotY, _transform->rotZ, _transform->rotW), 
					btVector3(_transform->posX,_transform->posY,_transform->posZ)));
	btGhostPairCallback* actorGhostPairCallback = new btGhostPairCallback();
   	PhysicsManager::instance()->overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(actorGhostPairCallback);

	controller = new OiJE::CharacterController(_ghostObject, _collisionShape, step_height);

	dynamics_world->addCollisionObject(_ghostObject, col_mask, col_to_masks);
  dynamics_world->addAction(controller);



   //------------------------------ ground - static plane ------------------------------
   btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0), 1);
   btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,-1,0)));
   btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0,0,0));
   btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
   dynamics_world->addRigidBody(groundRigidBody, 1 << 0, 1 << 1);
}

FPSBoxController::~FPSBoxController()
{
	dynamics_world->removeCollisionObject((btCollisionObject*)_ghostObject);
	dynamics_world->removeAction(controller);

	delete fps_camera;
	delete _collisionShape;
	delete _ghostObject;
	delete controller;
}

void FPSBoxController::update()
{
	updateTransform();
	testCollision();
	detectInput();
	controller->updateAction(dynamics_world, GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
}

void FPSBoxController::detectInput()
{

	btQuaternion rot = _ghostObject->getWorldTransform().getRotation();
	btVector3 v = rot.getAxis();
	// btVector3 v = getAxis().normalize();

	// KC_SPACE
	if(can_move)
	{
		// if(InputManager::instance()->isKeyDown(OIS::KC_W))
		// {
			// controller->setVelocityForTimeInterval(v * base_movement_speed * movement_speed_multiplier, 
				// GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		// }
		// else if(InputManager::instance()->isKeyDown(OIS::KC_W))
		// {
		// 	controller->setVelocityForTimeInterval(btVector3(0, 0, base_movement_speed * movement_speed_multiplier), 
		// 		GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		// }
		// if(InputManager::instance()->isKeyDown(OIS::KC_W))
		// {
		// 	controller->setVelocityForTimeInterval(btVector3(0, 0, base_movement_speed * movement_speed_multiplier), 
		// 		GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		// }
		// if(InputManager::instance()->isKeyDown(OIS::KC_W))
		// {
		// 	controller->setVelocityForTimeInterval(btVector3(0, 0, base_movement_speed * movement_speed_multiplier), 
		// 		GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		// }
		// Ogre::Vector3 dir = ((Camera*)fps_camera)->camera->getDirection();
		// controller->setWalkDirection(btVector3(dir.x, 0, dir.z));

		if(InputManager::instance()->isKeyDown(OIS::KC_W))
		{
			controller->setVelocityForTimeInterval(v * base_movement_speed * movement_speed_multiplier, 
				GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		}
		else if(InputManager::instance()->isKeyDown(OIS::KC_S))
		{
			controller->setVelocityForTimeInterval(btVector3(0, 0, -base_movement_speed * movement_speed_multiplier), 
				GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		}
		else if(InputManager::instance()->isKeyDown(OIS::KC_A))
		{
			controller->setVelocityForTimeInterval(btVector3(-base_movement_speed * movement_speed_multiplier, 0, 0), 
				GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		}
		else if(InputManager::instance()->isKeyDown(OIS::KC_D))
		{
			controller->setVelocityForTimeInterval(btVector3(base_movement_speed * movement_speed_multiplier, 0, 0), 
				GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
		}
		
	}
}

void FPSBoxController::updateTransform()
{
	btTransform ghost_transform = _ghostObject->getWorldTransform();

	btVector3 pos = ghost_transform.getOrigin();

	_transform->posX = pos.x();
	_transform->posY = pos.y();
	_transform->posZ = pos.z();

	btQuaternion rot = ghost_transform.getRotation();

	controller->setTurnAngle(1);

	_ghostObject->setWorldTransform(btTransform(btQuaternion(_transform->rotX, _transform->rotY, _transform->rotZ, _transform->rotW), pos));
}

void FPSBoxController::testCollision()
{
	if(onCollision != NULL)
	{

	  btManifoldArray   manifoldArray;
	  btBroadphasePairArray& pairArray = _ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
	  int numPairs = pairArray.size();

	  for (int i=0;i<numPairs;i++)
	  {
	     manifoldArray.clear();

	     const btBroadphasePair& pair = pairArray[i];

	     //unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
	     btBroadphasePair* collisionPair = dynamics_world->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
	     if (!collisionPair)
	        continue;

	     if (collisionPair->m_algorithm)
	        collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

	     for (int j=0;j<manifoldArray.size();j++)
	     {
	        btPersistentManifold* manifold = manifoldArray[j];
	        btScalar directionSign = manifold->getBody0() == _ghostObject ? btScalar(-1.0) : btScalar(1.0);
	        for (int p=0;p<manifold->getNumContacts();p++)
	        {
	         	const btManifoldPoint& pt = manifold->getContactPoint(p);
	            if (pt.getDistance()<0.f)
				{
					GameObject* gameObject;
					if(manifold->getBody0() == _ghostObject) {
						btCollisionObject* obj = (btCollisionObject*)manifold->getBody1();
						gameObject = (GameObject*)(obj->getUserPointer());
					} else {
						assert(manifold->getBody1() ==_ghostObject && "body does not match either collision object");
						btCollisionObject* obj = (btCollisionObject*)manifold->getBody0();
						gameObject = (GameObject*)(obj->getUserPointer());
					}
					onCollision((pt.m_positionWorldOnB), (pt.m_normalWorldOnB), gameObject);
				}
	        }
	     }
	  }
	}
}

