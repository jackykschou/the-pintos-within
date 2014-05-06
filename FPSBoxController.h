#ifndef __FPSBoxController_h_
#define __FPSBoxController_h_

#include "common.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Component.h"
#include "PhysicsManager.h"
#include "Component.h"
#include "oije_charactercontroller.h"
#include "Transform.h"
#include "FPSCamera.h"

#define JET_PACK_SPEED 0.5

class PlayerCharacter;

class FPSBoxController : Component
{
protected:
	Transform* _transform;
	btConvexShape* _collisionShape;
	btDiscreteDynamicsWorld* dynamics_world;

	void testCollision();
	void updateTransform();
	void transformUpdate();
	void detectInput();

public:
	double base_movement_speed;

	PlayerCharacter* player;

	bool is_yourself;

	double jet_pack_max;
	double jet_pack_current;

	bool is_jet_started;
	bool is_jet_packing;
	bool is_walking;
	bool is_running;

	FPSCamera* fps_camera;
	OiJE::CharacterController* controller;
	btPairCachingGhostObject* _ghostObject;

	bool can_move;
	double movement_speed_multiplier;
	float slowDown;
	float speedUp;

	float jet_slowDown;
	float jet_speedUp;
	float jet_bonus_speed;

	int jet_cost;

	btVector3 currVel;
	btVector3 jetVel;

	void (*onCollision)(btVector3, btVector3, GameObject*);

	FPSBoxController(bool, GameObject*, std::string, double, const btVector3&, btScalar, int, int, Ogre::SceneNode*);
	virtual ~FPSBoxController();

	virtual void update();
};

#endif // #ifndef __FPSBoxController_h_
