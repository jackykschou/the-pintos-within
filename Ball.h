#ifndef __Ball_h_
#define __Ball_h_

#include "common.h"

#include "PhysicsManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "SphereRigidbody.h"

class Transform;

class Ball : GameObject
{
	protected:
		Transform* _transform;
		float dead_timer;

	public:
		Ball(std::string tag, Scene* scene, 
				int mask, int col_mask, std::string mesh_name,
				float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
				float scaleX, float scaleY, float scaleZ, btVector3 init_force);
		virtual ~Ball();

		Mesh* mesh;
		SphereRigidbody* rigidbody;

		virtual void update();
};

#endif // #ifndef __Ball_h_