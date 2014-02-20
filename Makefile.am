ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= common.h Singleton.h PhysicsManager.h ResourcesManager.h GraphicsManager.h SceneManager.h Scene.h GameObject.h Component.h Transform.h Camera.h Mesh.h FPSCamera.h FPSController.h InputManager.h GUIManager.h AudioManager.h TheEngine.h
bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir) -std=c++0x
OgreApp_SOURCES= Application.cpp PhysicsManager.cpp ResourcesManager.cpp GraphicsManager.cpp SceneManager.cpp Scene.cpp GameObject.cpp Component.cpp Transform.cpp Camera.cpp Mesh.cpp FPSCamera.cpp FPSController.cpp InputManager.cpp GUIManager.cpp AudioManager.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) -lSDL_mixer
EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign

