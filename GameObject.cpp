#include "GameObject.h"

void setId(int);

Component* GameObject::addComponent(Component* component)
{
	GameObject
}

void GameObject::update()
{
	for(auto c in _components)
	{
		c->update();
	}
}