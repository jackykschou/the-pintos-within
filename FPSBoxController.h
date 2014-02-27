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
	FPSCamera* fps_camera;
	OiJE::CharacterController* controller;

	bool can_move;
	double movement_speed_multiplier;
	float slowDown;
	float speedUp;

	btVector3 currVel;

	void (*onCollision)(btVector3, btVector3, GameObject*);

	FPSBoxController(GameObject*, std::string, double, const btVector3&, btScalar, int, int);
	virtual ~FPSBoxController();

	virtual void update();
};

#endif // #ifndef __FPSBoxController_h_
