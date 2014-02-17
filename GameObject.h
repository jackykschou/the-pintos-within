#ifndef __GameObject_h_
#define __GameObject_h_

#include "common.h"

class GameObject
{
protected:
	int _id;
	std::vector<Component*> _components;

public:
	bool active;

	GameObject();
	~GameObject();

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
		if()
			return NULL;
		else
			return vect
	}

	template<typename T>
	bool containsComponent<T>()
	{
		return getComponent<T>() != NULL;
	}

	void setId(int);
	Component* addComponent(Component* component);
	Component* removeComponent(Component* component);
	void update();

};

#endif // #ifndef __GameObject_h_