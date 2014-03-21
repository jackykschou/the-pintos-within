#ifndef __PlayerBox_h_
#define __PlayerBox_h_

#include "common.h"

#include "GraphicsManager.h"

#include "Component.h"
#include "PlayerCharacter.h"
#include "Mesh.h"
#include "BoxRigidbody.h"

class PlayerCharacter;

class PlayerBox : Component
{
	public:
	PlayerCharacter* player;
	Mesh* mesh;
	BoxRigidbody* rigidbody;

	Ogre::Entity* entity;
  	Ogre::SceneNode* node;

	PlayerBox(PlayerCharacter*, std::string, float, float, float, 
		float, float, float, float,
		float, float, float, int, int);
	virtual ~PlayerBox();

	virtual void update();
};

#endif