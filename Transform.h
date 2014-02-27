#ifndef __Transform_h_
#define __Transform_h_

#include "common.h"

#include "Component.h"
#include "GameObject.h"

class GameObject;

class Transform : public Component
{
protected:

public:
	Transform(GameObject*);
	virtual ~Transform();

	float posX;
	float posY;
	float posZ;

	float rotX;
	float rotY;
	float rotZ;
	float rotW;

	float scaleX;
	float scaleY;
	float scaleZ;
};

#endif // #ifndef __Transform_h_