#ifndef __Ball_h_
#define __Ball_h_

#include "common.h"

#include "PhysicsManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "SphereRigidbody.h"
#include "GameState.h"
#include "AudioManager.h"
#include "Debouncer.h"

#define DEBOUNCE_BALL 300

class Transform;

class Ball : GameObject
{
	protected:
		Transform* _transform;

	public:
		Ball(std::string tag, Scene* scene, 
			int mask, int col_mask, std::string mesh_name,
			float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, btVector3 init_force);
		virtual ~Ball();

		Mesh* mesh;
		SphereRigidbody* rigidbody;
		Debouncer *debouncer;
		int collided;

		virtual void update();
};

#endif // #ifndef __Ball_h_