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

class FPSBoxController : Component
{
protected:
	double base_movement_speed;

	Transform* _transform;
	btPairCachingGhostObject* _ghostObject;
	btConvexShape* _collisionShape;
	btDiscreteDynamicsWorld* dynamics_world;

	void testCollision();
	void updateTransform();
	void detectInput();

public:
	double jet_pack_max;
	double jet_pack_current;

	bool is_jet_packing;
	bool is_walking;
	bool is_running;

	FPSCamera* fps_camera;
	OiJE::CharacterController* controller;

	bool can_move;
	double movement_speed_multiplier;
	float slowDown;
	float speedUp;

	float jet_slowDown;
	float jet_speedUp;
	float jet_bonus_speed;

	btVector3 currVel;
	btVector3 jetVel;

	void (*onCollision)(btVector3, btVector3, GameObject*);

	FPSBoxController(GameObject*, std::string, double, const btVector3&, btScalar, int, int, Ogre::SceneNode*);
	virtual ~FPSBoxController();

	virtual void update();
};

#endif // #ifndef __FPSBoxController_h_
