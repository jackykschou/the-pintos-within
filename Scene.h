#ifndef __Scene_h_
#define __Scene_h_

#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Component.h"
#include "GameObject.h"

class Camera;
class Transform;
class GameObject;

class Scene
{
protected:
		std::vector<GameObject*> _game_objects;
		int _game_object_id_assigner;
public:
		Camera* main_camera;
		int id;
		std::string name;
		btDiscreteDynamicsWorld* physics_world;
		Ogre::SceneManager* manager;

		Scene(std::string);
		~Scene();

		void addGameObject(GameObject*);
		void removeGameObject(GameObject*);
		bool containsGameObject(GameObject*);
		std::vector<GameObject*> getGameObjectsOfTag(std::string);
		void update();
};

#endif // #ifndef __Scene_h_

