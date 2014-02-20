#ifndef __SceneManager_h_
#define __SceneManager_h_

#include "common.h"

#include "Scene.h"

class Scene;

class SceneManager: public Singleton<SceneManager>
{
public:
	Scene* current_scene;

   ~SceneManager();

	void initialize();    
    int addScene(Scene*);
    void removeScene(Scene*);
    Scene* getScene(std::string);
    void changeCurrentScene(Scene*);

protected:
    std::vector<Scene*> _scenes;
    int _scene_id_assigner;
};

#endif // #ifndef __SceneManager_h_