#ifndef __Component_h_
#define __Component_h_

#include "GameObject.h"

class Component
{
private:
	GameObject _gameObject;
	
public:
	Component();
	virtual ~Component();

	virtual void Start();
	virtual void Update();
};

#endif // #ifndef __Component_h_