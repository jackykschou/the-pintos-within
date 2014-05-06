#ifndef __Hair_h_
#define __Hair_h_

#include "common.h"

#include "Component.h"
#include "PlayerCharacter.h"
#include "Debouncer.h"
#include "PlayerBox.h"

#include "NetworkManager.h"

class PlayerCharacter;
class PlayerBox;

class Hair : protected Component
{
public:
	PlayerCharacter* player;

	Ogre::AnimationState *running_animation_state;
	Ogre::AnimationState *idle_animation_state;
	Ogre::AnimationState *shooting_animation_state;
	Ogre::AnimationState *reload_animation_state;
	Ogre::AnimationState *jumping_animation_state;
	Ogre::AnimationState *die_animation_state;

	int hair_id;

	Ogre::Entity* entity;
  	Ogre::SceneNode* node;
  
	Hair(PlayerCharacter*, std::string, int, 
			float, float, float, float, float, float, float,
			float, float, float);
	virtual ~Hair();

	virtual void update();
	
protected:
	Transform* _transform;
};

#endif // #ifndef __Hair_h_