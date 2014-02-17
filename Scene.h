#ifndef __Scene_h_
#define __Scene_h_

#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

class Scene
{
protected:
		std::vector<GameObject*> _game_objects;
		int _game_object_id_assigner = 0;
public:
		btDiscreteDynamicsWorld* physics_world;
		Ogre::SceneManager* manager;

		Scene();
		~Scene();

		void addGameObject(GameObject*);
		void removeGameObject(GameObject*);
		bool containsGameObject(GameObject*);
		void update();
};

#endif // #ifndef __Scene_h_

