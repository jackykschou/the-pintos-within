#include "GameObject.h"

GameObject::GameObject(std::string t, Scene* s)
{
	tag = t;
	scene = s;
	scene->addGameObject(this);
	active = true;
}

GameObject::~GameObject()
{
	for(auto c : components)
	{
		delete c;
	}
}

Component* GameObject::addComponent(Component* component)
{
	component->id = component_id_assigner;
	components.push_back(component);
}

void GameObject::removeComponent(Component* component)
{
	int i = 0;
	for(auto c : components)
	{
		if(c->id == component->id)
		{
			components.erase(components.begin() + i);
			delete c;
		}
		++i;
	}
}

void GameObject::update()
{
	for(auto c : components)
	{
		if(c->active)
			c->update();
	}
}
