#include "FPSBoxController.h"

FPSBoxController::FPSBoxController(bool is_yourself_p, GameObject* gameObject, std::string camera_name, double camera_offset, 
														const btVector3& boxHalfExtents, btScalar step_height, 
														int col_mask, int col_to_masks, Ogre::SceneNode* node) : Component(gameObject), currVel(0, 0, 0), jetVel(0, 0, 0)
{
	is_yourself = is_yourself_p;

	jet_pack_max = 5000;
	jet_pack_current = jet_pack_max;

	is_jet_packing = false;
	is_walking = false;


	onCollision = NULL;
	can_move = true;

	base_movement_speed = 1.0f;
	jet_bonus_speed = 2.0f;

	movement_speed_multiplier = 1;

	slowDown = 0.92f;
	speedUp = 0.02f;

	jet_slowDown = 0.92f;
	jet_speedUp = 0.04f;

	_transform = gameObject->getComponent<Transform>();

	if(is_yourself)
	{
		fps_camera = new FPSCamera(gameObject, camera_name, camera_offset, node);
	}

	_collisionShape = new btBoxShape(boxHalfExtents);
	dynamics_world = _gameObject->scene->physics_world;

	_ghostObject = new btPairCachingGhostObject();
	_ghostObject->setUserPointer(gameObject);
	_ghostObject->setCollisionShape(_collisionShape);
	_ghostObject->setWorldTransform(btTransform(btQuaternion(_transform->rotX, _transform->rotY, _transform->rotZ, _transform->rotW), 
					btVector3(_transform->posX,_transform->posY,_transform->posZ)));
	_ghostObject->setUserPointer(gameObject);
	btGhostPairCallback* actorGhostPairCallback = new btGhostPairCallback();
   	PhysicsManager::instance()->overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(actorGhostPairCallback);

	controller = new OiJE::CharacterController(_ghostObject, _collisionShape, step_height);

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
	if(is_yourself)
	{
		testCollision();
		detectInput();
	}
	controller->updateAction(dynamics_world, GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
}

void FPSBoxController::detectInput()
{

	btQuaternion rot = _ghostObject->getWorldTransform().getRotation();
	Ogre::Vector3 v0 = ((Camera*)fps_camera)->camera->getDirection();
	btVector3 v = btVector3(v0.x, 0, v0.z);

	btVector3 tempDir = btVector3(0,0,0);
	btVector3 jetTempDir = btVector3(0,0,0);

	is_walking = false;
	is_jet_packing = false;
	is_running = false;

	if(can_move)
	{
		if(InputManager::instance()->isKeyPressed(OIS::KC_SPACE) && !is_jet_packing)
		{
			controller->jump();
			if(controller->isJumping())
				is_walking = true;
		}

		if(InputManager::instance()->isMouseRightDown() && (jet_pack_current > 10))
		{
			jet_pack_current -= 10;
			is_jet_packing = true;
			jetTempDir += btVector3(0,JET_PACK_SPEED,0);
			movement_speed_multiplier = 2.0f;
		}
		else if(jet_pack_current < jet_pack_max)
		{
			jet_pack_current = ((jet_pack_current + 2) >= jet_pack_max) ? (jet_pack_max) : (jet_pack_current + 2);
			movement_speed_multiplier = 1.0f;
		}

		if(InputManager::instance()->isKeyDown(OIS::KC_A))
		{
			tempDir += -v.cross(btVector3(0,1,0));
			is_walking = true;
		}

		if(InputManager::instance()->isKeyDown(OIS::KC_D))
		{
			tempDir += v.cross(btVector3(0,1,0));
			is_walking = true;
		}
		if(InputManager::instance()->isKeyDown(OIS::KC_W))
		{
			tempDir += v;
			is_walking = true;
		}
		if(InputManager::instance()->isKeyDown(OIS::KC_S))
		{
			tempDir += -v;
			is_walking = true;
		}

		if(InputManager::instance()->isKeyDown(OIS::KC_LSHIFT) && !is_jet_packing && controller->onGround() && !is_jet_packing && is_walking)
		{
			is_running = true;
			movement_speed_multiplier = 1.5f;
		}
		else if(!controller->onGround() && !is_jet_packing && !controller->isJumping())
		{
			movement_speed_multiplier = 0.5f;
		}
		else {
			movement_speed_multiplier = 1.0f;
		}
	}

		if(controller->isJumping() && !is_jet_packing)
		{
		}
		else if(!tempDir.isZero()) {
		    tempDir = tempDir.normalize();
			currVel += tempDir * speedUp;
			if (currVel.length() > 1) {
				currVel = currVel.normalize();
			}
		}
		else 
		{
			if (currVel.length() < speedUp) {
				currVel = btVector3(0,0,0);
			}
			else
				currVel *= slowDown;
		}

		if(is_jet_packing)
		{
			jetTempDir = jetTempDir.normalize();
			jetVel += jetTempDir * jet_speedUp;
			if (jetVel.length() > 1) {
				jetVel = jetVel.normalize();
			}
		}
		else 
		{
			if (jetVel.length() < jet_speedUp) {
				jetVel = btVector3(0,0,0);
			}
			else
				jetVel *= jet_slowDown;
		}

		controller->setWalkDirection((currVel + (jetVel * jet_bonus_speed)) * base_movement_speed * movement_speed_multiplier);
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
