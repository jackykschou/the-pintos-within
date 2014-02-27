#ifndef __Ball_h_
#define __Ball_h_

#include "common.h"

#include "Component.h"
#include "PhysicsManager.h"
#include "Transform.h"

class Transform;

class Ball : GameObject
{
	protected:
		Transform* _transform;

	public:
		Ball(GameObject*);
		virtual ~Ball();

		Mesh* mesh;
		SphereRigidbody* rigidbody;

		virtual void update();
};

#endif // #ifndef __Ball_h_