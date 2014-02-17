#ifndef __SceneManager_h_
#define __SceneManager_h_

#include "Scene.h"

class : Singleton<SceneManager>
{
public:
	Scene* _current_scene;

    SceneManager();
    ~SceneManager();

    int addScene(Scene*);
    void removeScene(int);
    Scene* GetScene(int);
    void updateCurrentScene();

protected:
    std::vector<Scene*> _scenes;
};

#endif // #ifndef __SceneManager_h_