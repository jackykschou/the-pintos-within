#ifndef __HitBox_h_
#define __HitBox_h_

#include "common.h"

#include "GraphicsManager.h"

#include "Component.h"
#include "PlayerCharacter.h"
#include "Mesh.h"
#include "BoxRigidbody.h"

class HitBox : PlayerBox
{
protected:
	float damage_multiplier;

public:
	HitBox(PlayerCharacter*, std::string, float, float, float, 
		float, float, float, float,
		float, float, float, float);

	int getDamage(int);
};

#endif