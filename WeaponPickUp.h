#ifndef __WeaponPickUp_h_
#define __WeaponPickUp_h_

#include "common.h"

#include "PlayerPickUp.h"

class WeaponPickUp : public PlayerPickUp
{
	protected:
		Transform* _transform;

	public:
		WeaponPickUp(std::string tag, Scene* scene, std::string mesh_name,
			float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, uint32_t);

		uint32_t weapon_id;

		virtual void onPicked(PlayerCharacter*);
};
#endif
