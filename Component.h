#ifndef __Component_h_
#define __Component_h_

#include "GameObject.h"

#include "common.h"

class GameObject;

class Component
{
protected:
	GameObject* _gameObject;
	
public:
	int id;
	bool active;

	Component(GameObject*);
	~Component();

	virtual void update();
};

#endif // #ifndef __Component_h_

