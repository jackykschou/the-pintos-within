#include "FPSBoxController.h"

FPSBoxController::FPSBoxController(GameObject* gameObject, std::string camera_name, double camera_offset, 
														const btVector3& boxHalfExtents, btScalar step_height, 
														int col_mask, int col_to_masks) : Component(gameObject), currVel(0, 0, 0)
{
	onCollision = NULL;
	can_move = true;

	base_movement_speed = .05;

	movement_speed_multiplier = 1;
	slowDown = 0.995;
	speedUp = 0.005;

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

	controller = new OiJE::CharacterController(_ghostObject, _collisionShape, 1);

	dynamics_world->addCollisionObject(_ghostObject, col_mask, col_to_masks);
  	dynamics_world->addAction(controller);
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
	Ogre::Vector3 v0 = ((Camera*)fps_camera)->camera->getDirection();
	btVector3 v = btVector3(v0.x, v0.y, v0.z);

	btVector3 tempDir = btVector3(0,0,0);


	if(can_move)
	{
		if(InputManager::instance()->isKeyDown(OIS::KC_A))
		{
			tempDir += -v.cross(btVector3(0,1,0));
		}

		if(InputManager::instance()->isKeyDown(OIS::KC_D))
		{
			tempDir += v.cross(btVector3(0,1,0));
		}
		if(InputManager::instance()->isKeyDown(OIS::KC_W))
		{
			tempDir += v;
		}
		if(InputManager::instance()->isKeyDown(OIS::KC_S))
		{
			tempDir += -v;
		}

		if(InputManager::instance()->isKeyDown(OIS::KC_SPACE))
		{
			controller->jump();
		}
	}

		//UPDATE MOVEMENT DIRECTION

		
		if(!tempDir.isZero()) {
		    tempDir = tempDir.normalize();
			currVel += tempDir * speedUp;
			if (currVel.length() > 1) {
				currVel = currVel.normalize();
			}
		}
		else {
			if (currVel.length() < speedUp) {
				currVel = btVector3(0,0,0);
			}
			else
				currVel *= slowDown;
		}
		controller->setWalkDirection(currVel * base_movement_speed * movement_speed_multiplier);
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
