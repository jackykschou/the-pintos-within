#ifndef __HairPickUp_h_
#define __HairPickUp_h_

#include "common.h"

#include "PlayerPickUp.h"

class HairPickUp : public PlayerPickUp
{
	protected:
		Transform* _transform;

	public:
		HairPickUp(std::string tag, Scene* scene, std::string mesh_name,
			float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, uint32_t, int);

		uint32_t hair_id;

		virtual void onPicked(PlayerCharacter*);
};
#endif
