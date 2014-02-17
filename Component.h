#ifndef __Component_h_
#define __Component_h_

#include "GameObject.h"

class Component
{
private:
	GameObject _gameObject;
	
public:
	~Component();

	virtual void initialize(GameObject);
	virtual void update();
	virtual void finalize();
};

#endif // #ifndef __Component_h_