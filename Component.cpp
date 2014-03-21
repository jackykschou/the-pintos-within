#include "Component.h"

Component::Component(GameObject* gameObject)
{
	_gameObject = gameObject;
	gameObject->addComponent(this);
	active = true;
}

Component::~Component()
{
}

void Component::update()
{
}


