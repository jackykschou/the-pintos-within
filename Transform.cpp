#include "Transform.h"

Transform::Transform(GameObject* gameObject) : Component(gameObject)
{
	posX = 0;
	posY = 0;
	posZ = 0;

	rotX = 0;
	rotY = 0;
	rotZ = 0;
	rotW = 1;

	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
}

Transform::~Transform()
{
}