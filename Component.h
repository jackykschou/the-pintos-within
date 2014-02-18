#ifndef __Component_h_
#define __Component_h_

#include "common.h"

class Component
{
private:
	GameObject* _gameObject;
	
public:
	int id;
	bool active;

	virtual Component();
	virtual ~Component();

	virtual void update();
};

#endif // #ifndef __Component_h_