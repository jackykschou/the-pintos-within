ACLOCAL_AMFLAGS= -I m4

GUIHEADERS= Gui3D/Gorilla.h Gui3D/Gui3D.h Gui3D/Gui3DButton.h Gui3D/Gui3DCaption.h Gui3D/Gui3DCheckbox.h Gui3D/Gui3DCheckboxSprite.h Gui3D/Gui3DCheckboxText.h Gui3D/Gui3DClock.h Gui3D/Gui3DCombobox.h Gui3D/Gui3DConfig.h Gui3D/Gui3DContainer.h Gui3D/Gui3DDebugPanelColors.h Gui3D/Gui3DFunctorBase.h Gui3D/Gui3DInlineSelector.h Gui3D/Gui3DListbox.h Gui3D/Gui3DMemberFunction.h Gui3D/Gui3DMultipleElementsSelector.h Gui3D/Gui3DPanel.h Gui3D/Gui3DPanelColors.h Gui3D/Gui3DPanelContainer.h Gui3D/Gui3DPanelElement.h Gui3D/Gui3DPanelElementCallback.h Gui3D/Gui3DProgressBar.h Gui3D/Gui3DScreenPanel.h Gui3D/Gui3DScrollBar.h Gui3D/Gui3DStringCursorEdit.h Gui3D/Gui3DTextZone.h Gui3D/Gui3DUtils.h Gui3D/Gui3DVerticalSelector.h

GUISOURCE= Gui3D/Gorilla.cpp Gui3D/Gui3D.cpp Gui3D/Gui3DButton.cpp Gui3D/Gui3DCaption.cpp Gui3D/Gui3DCheckbox.cpp Gui3D/Gui3DCheckboxSprite.cpp Gui3D/Gui3DCheckboxText.cpp Gui3D/Gui3DClock.cpp Gui3D/Gui3DCombobox.cpp Gui3D/Gui3DContainer.cpp Gui3D/Gui3DDebugPanelColors.cpp Gui3D/Gui3DInlineSelector.cpp Gui3D/Gui3DListbox.cpp Gui3D/Gui3DMultipleElementsSelector.cpp Gui3D/Gui3DPanel.cpp Gui3D/Gui3DPanelContainer.cpp Gui3D/Gui3DPanelElement.cpp Gui3D/Gui3DProgressBar.cpp Gui3D/Gui3DScreenPanel.cpp Gui3D/Gui3DScrollBar.cpp Gui3D/Gui3DStringCursorEdit.cpp Gui3D/Gui3DTextZone.cpp Gui3D/Gui3DVerticalSelector.cpp 

noinst_HEADERS=  $(GUIHEADERS) common.h Singleton.h PhysicsManager.h ResourcesManager.h GraphicsManager.h SceneManager.h Scene.h Component.h Transform.h GameObject.h Camera.h Mesh.h FPSCamera.h FPSBoxController.h InputManager.h GUIManager.h AudioManager.h TheEngine.h Rigidbody.h btCollisionObjectWrapper.h SphereRigidbody.h Debug.h oije_charactercontroller.h BoxRigidbody.h DotSceneLoader.h rapidxml.hpp rapidxml_iterators.hpp rapidxml_print.hpp rapidxml_utils.hpp Block.h Ball.h BallSpawner.h GameState.h Debouncer.h ParticleManager.h PlayerCharacter.h Weapon.h PlayerBox.h HitBox.h Pistol.h PlayerSpawner.h HeartbeatPacket.h NetworkManager.h GameServer.h GameClient.h
bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir) -std=c++0x
OgreApp_SOURCES=  $(GUISOURCE) Application.cpp PhysicsManager.cpp ResourcesManager.cpp GraphicsManager.cpp SceneManager.cpp Scene.cpp GameObject.cpp Component.cpp Transform.cpp Camera.cpp Mesh.cpp FPSCamera.cpp FPSBoxController.cpp InputManager.cpp GUIManager.cpp AudioManager.cpp Rigidbody.cpp SphereRigidbody.cpp oije_charactercontroller.cpp BoxRigidbody.cpp DotSceneLoader.cpp Block.cpp Ball.cpp BallSpawner.cpp GameState.cpp Debouncer.cpp ParticleManager.cpp PlayerCharacter.cpp Weapon.cpp PlayerBox.cpp HitBox.cpp Pistol.cpp PlayerSpawner.cpp HeartbeatPacket.cpp NetworkManager.cpp GameServer.cpp GameClient.cpp

OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) -lSDL_mixer -lSDL_net
EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign

