#ifndef __GameObject_h_
#define __GameObject_h_

#include "common.h"

#include "Component.h"
#include "Scene.h"

class Scene;
class Component;

class GameObject
{
protected:
	std::vector<Component*> _components;
	int component_id_assigner;

public:
	std::string tag;
	int id;
	bool active;
	Scene* scene;

	GameObject(std::string, Scene*);
	~GameObject();

	template<typename T>
	T* getComponent()
	{
		T* return_component = NULL;
		for(auto c: _components)
		{
			return_component = dynamic_cast<T*>(c);
			if(return_component != NULL)
				return return_component;
		}
		return NULL;
	}

	template<typename T>
	std::vector<T*> getComponents()
	{
		std::vector<T*> components;
		for(auto c: _components)
		{
			T* component = dynamic_cast<T*>(c);
			if(component != NULL)
				components.push_back(component);
		}
		return components;
	}

	template<typename T>
	bool containsComponent()
	{
		return getComponent<T>() != NULL;
	}

	Component* addComponent(Component* component);
	void removeComponent(Component* component);

	virtual void update();

};

#endif // #ifndef __GameObject_h_

