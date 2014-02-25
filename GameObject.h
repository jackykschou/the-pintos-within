#ifndef __GameObject_h_
#define __GameObject_h_

#include "common.h"

#include "Scene.h"
#include "Component.h"

class Scene;
class Component;

class GameObject
{
protected:
	int component_id_assigner;

public:
	std::string tag;
	int id;
	bool active;
	Scene* scene;
	std::vector<Component*> components;

	GameObject(std::string, Scene*);
	virtual ~GameObject();

	template<typename T>
	T* getComponent()
	{
		T* return_component = NULL;
		for(auto c: components)
		{
			return_component = dynamic_cast<T*>(c);
			if(return_component != NULL)
				return return_component;
		}

		LOG("Warning, does not contains component");
		
		return NULL;
	}

	template<typename T>
	std::vector<T*> getComponents()
	{
		std::vector<T*> temp_components;
		for(auto c: components)
		{
			T* component = dynamic_cast<T*>(c);
			if(component != NULL)
				temp_components.push_back(component);
		}
		return temp_components;
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

