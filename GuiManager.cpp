#include "GuiManager.h"

GuiManager::GuiManager(){
}
GuiManager::~GuiManager(){
  CEGUI::OgreRenderer::destroySystem();
}
void GuiManager::Update(const Ogre::FrameEvent& event){}
void GuiManager::Initialize(std::string applicationName){
  _renderer=&CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );
  CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Window* myRoot = wmgr.createWindow( "DefaultWindow", "root" );
  CEGUI::System::getSingleton().setGUISheet( myRoot );
  CEGUI::FrameWindow* fWnd = static_cast<CEGUI::FrameWindow*>(
      wmgr.createWindow( "TaharezLook/FrameWindow", "testWindow" ));
  myRoot->addChildWindow( fWnd );
  // position a quarter of the way in from the top-left of parent.
  fWnd->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.25f, 0 ), CEGUI::UDim( 0.25f, 0 ) ) );
  // set size to be half the size of the parent
  fWnd->setSize( CEGUI::UVector2( CEGUI::UDim( 0.5f, 0 ), CEGUI::UDim( 0.5f, 0 ) ) );
  fWnd->setText(applicationName);

  CEGUI::ProgressBar* healthBar = static_cast<CEGUI::ProgressBar*>(
      wmgr.createWindow("TaharezLook/ProgressBar","HealthBar"));
  myRoot->addChildWindow(healthBar);
  healthBar->setProgress(0.75f);
  healthBar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 10.0f), CEGUI::UDim( 0.92f, 0.0f)));
  healthBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.02f, 0.0f)));

  CEGUI::ProgressBar* fuelBar = static_cast<CEGUI::ProgressBar*>(
      wmgr.createWindow("TaharezLook/ProgressBar","FuelBar"));
  myRoot->addChildWindow(fuelBar);
  fuelBar->setProgress(0.2f);
  fuelBar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 10.0f), CEGUI::UDim( 0.96f, 0.0f)));
  fuelBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.02f, 0.0f)));
}
CEGUI::MouseButton GuiManager::convertButton(OIS::MouseButtonID buttonId){
  switch(buttonId){
    case OIS::MB_Left:
	  return CEGUI::LeftButton;
    case OIS::MB_Right:
      return CEGUI::RightButton;
    case OIS::MB_Middle:
      return CEGUI::MiddleButton;
    default:
      return CEGUI::LeftButton;
  }
}
