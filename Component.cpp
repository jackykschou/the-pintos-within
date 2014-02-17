#include "Component.h"

Component::~Component()
{
	Finalize();
}

void Component::initialize(GameObject* gameObject)
{
	_gameObject = gameObject;
	gameObject->addComponent(this);
}

void Component::update(){}
void Component::finalize()
{
	gameObject->removeComponent(this);
}