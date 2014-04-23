#ifndef __PlayerPickUp_h_
#define __PlayerPickUp_h_

#include "common.h"

#include "GameObject.h"
class Transform;
class PlayerCharacter;
class Mesh;
class SphereRigidbody;
class Debouncer;

class PlayerPickUp : GameObject
{
	protected:
		Transform* _transform;

	public:
		PlayerPickUp(std::string tag, Scene* scene, std::string mesh_name,
			float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ);
		virtual ~PlayerPickUp();

		Mesh* mesh;
		SphereRigidbody* rigidbody;
		int collided;
		Debouncer* pick_debouncer;

		virtual void update();
		virtual void onPicked(PlayerCharacter*);
};

#endif
