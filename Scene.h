#ifndef __Scene_h_
#define __Scene_h_

#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

#include "common.h"

class Scene
{
protected:
		std::vector<GameObject*> _game_objects;
		int _game_object_id_assigner = 0;
public:
		int id;
		std::string name;
		btDiscreteDynamicsWorld* physics_world;
		Ogre::SceneManager* manager;

		Scene(std::string);
		~Scene();

		void addGameObject(GameObject*);
		void removeGameObject(GameObject*);
		bool containsGameObject(GameObject*);
		std::vector<GameObject*> Scene::getGameObjectsOfTag(std::string);
		void update();
};

#endif // #ifndef __Scene_h_

