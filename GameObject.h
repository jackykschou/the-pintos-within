#ifndef __GameObject_h_
#define __GameObject_h_

#include "common.h"

class GameObject
{
protected:
	std::vector<Component*> _components;
	int component_id_assigner;

public:
	int id;
	bool active;
	Scene* scene;

	virtual GameObject();
	virtual ~GameObject();

	template<typename T>
	T* getComponent<T>()
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
	std::vector<T*> getComponents<T>()
	{
		std::vector<T*> components;
		for(auto c: _components)
		{
			T* component = dynamic_cast<T*>(c);
			if(component != NULL)
				components.push_back(component);
		}
		if(components.empty())
			return NULL;
		else
			return components;
	}

	template<typename T>
	bool containsComponent<T>()
	{
		return getComponent<T>() != NULL;
	}

	Component* addComponent(Component* component);
	void removeComponent(Component* component);

	virtual void update();

};

#endif // #ifndef __GameObject_h_