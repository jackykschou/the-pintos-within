ACLOCAL_AMFLAGS= -I m4

noinst_HEADERS= common.h Singleton.h PhysicsManager.h ResourcesManager.h GraphicsManager.h SceneManager.h Scene.h Component.h Transform.h GameObject.h Camera.h Mesh.h FPSCamera.h FPSBoxController.h InputManager.h AudioManager.h TheEngine.h Rigidbody.h btCollisionObjectWrapper.h SphereRigidbody.h Debug.h oije_charactercontroller.h BoxRigidbody.h DotSceneLoader.h rapidxml.hpp rapidxml_iterators.hpp rapidxml_print.hpp rapidxml_utils.hpp Block.h Ball.h BallSpawner.h GameState.h Debouncer.h ParticleManager.h PlayerCharacter.h Weapon.h PlayerBox.h HitBox.h Pistol.h PlayerSpawner.h HeartbeatPacket.h NetworkManager.h GameServer.h GameClient.h AckBuffer.h VitalPacket.h ParticlePacket.h GuiManager.h ShotGun.h AssaultRifle.h Blaster.h PlayerPickUp.h WeaponPickUp.h WeaponSpawner.h ChatManager.h Melee.h Hair.h HairPickUp.h
bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir) -std=c++0x
OgreApp_SOURCES= Application.cpp PhysicsManager.cpp ResourcesManager.cpp GraphicsManager.cpp SceneManager.cpp Scene.cpp GameObject.cpp Component.cpp Transform.cpp Camera.cpp Mesh.cpp FPSCamera.cpp FPSBoxController.cpp InputManager.cpp AudioManager.cpp Rigidbody.cpp SphereRigidbody.cpp oije_charactercontroller.cpp BoxRigidbody.cpp DotSceneLoader.cpp Block.cpp Ball.cpp BallSpawner.cpp GameState.cpp Debouncer.cpp ParticleManager.cpp PlayerCharacter.cpp Weapon.cpp PlayerBox.cpp HitBox.cpp Pistol.cpp PlayerSpawner.cpp HeartbeatPacket.cpp NetworkManager.cpp GameServer.cpp GameClient.cpp AckBuffer.cpp VitalPacket.cpp ParticlePacket.cpp GuiManager.cpp ShotGun.cpp AssaultRifle.cpp Blaster.cpp PlayerPickUp.cpp WeaponPickUp.cpp WeaponSpawner.cpp ChatManager.cpp Melee.cpp Hair.cpp HairPickUp.cpp

OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) -lSDL_mixer -lSDL_net
EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign

