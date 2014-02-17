#include "GameObject.h"

GameObject::GameObject(Scene* s) : _components()
{
	scene = s;
	scene->addGameObject(this);
	active = true;
}

GameObject::~GameObject()
{
	for(auto c in _components)
	{
			delete c;
	}
	scene->removeGameObject(this);
}

Component* GameObject::addComponent(Component* component)
{
	component->id = component_id_assigner;
	_components.push_back(component);
}

void GameObject::removeComponent(Component* component)
{
	int i = 0;
	for(auto c in _components)
	{
		if(c->id == component->id)
		{
			_components.erase(_components.begin() + i);
			delete c;
		}
		++i;
	}
}

void GameObject::update()
{
	for(auto c in _components)
	{
		if(c->active)
			c->update();
	}
}
