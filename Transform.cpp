#include "Transform.h"

Transform::Transform(GameObject* gameObject) : Component(gameObject)
{
	posX = 0;
	posY = 0;
	posZ = 0;

	rotX = 0;
	rotY = 0;
	rotZ = 0;
	rotW = 0;

	scaleX = 0;
	scaleY = 0;
	scaleZ = 0;
}

Transform::~Transform()
{
}