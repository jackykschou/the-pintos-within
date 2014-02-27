#include "InputManager.h"
#include "GraphicsManager.h"

InputManager::InputManager() :
	_mInputManager(0),
	_mMouse(0),
	_mKeyboard(0)
{}

// Sets up input devices and event callbacks
void InputManager::initialize() {

	_lastKeyPressedEvt = NULL;
    _lastKeyReleasedEvt = NULL;
    _lastMouseMovedEvt = NULL;
    _lastMousePressedEvt = NULL;
    _lastMouseReleasedEvt = NULL;

	OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    Ogre::RenderWindow* mWindow;

    // set up the render window
	mWindow = GraphicsManager::instance()->getRenderWindow();
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	// Set up the input thread
	_mInputManager = OIS::InputManager::createInputSystem(pl);

	// Set up the input devices
	_mKeyboard = static_cast<OIS::Keyboard*>(_mInputManager->createInputObject(OIS::OISKeyboard, true));
	_mMouse = static_cast<OIS::Mouse*>(_mInputManager->createInputObject(OIS::OISMouse, true));

	// Register for events
	_mMouse->setEventCallback(this);
	_mKeyboard->setEventCallback(this);

	_file = AudioManager::instance()->loadAudioFile("media/sounds/shotgun.wav");

	// Set initial mouse clipping size
    windowResized(mWindow);

	// Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

// Called every frame to snatch the input buffer from the device thread
void InputManager::capture() 
{
	flushBufferedEvents();

	_mKeyboard->capture();
	_mMouse->capture();

	if (InputManager::instance()->isKeyPressed(OIS::KC_ESCAPE)) {
        GraphicsManager::instance()->stopRendering();
    }

    if (InputManager::instance()->isKeyPressed(OIS::KC_Q)) {
        GUIManager::instance()->toggleTray();
    }

    if (InputManager::instance()->isMouseLeftClicked()) {
        AudioManager::instance()->play2DSound(_file, 0);
    }
}

void InputManager::flushBufferedEvents() {
	if (_lastMouseReleasedEvt != NULL) {
		delete _lastMouseReleasedEvt;
		_lastMouseReleasedEvt = NULL;
	}
	if (_lastMouseMovedEvt != NULL) {
		delete _lastMouseMovedEvt;
		_lastMouseMovedEvt = NULL;
	}
	if (_lastMousePressedEvt != NULL) {
		delete _lastMousePressedEvt;
		_lastMousePressedEvt = NULL;
	}
	if (_lastKeyReleasedEvt != NULL) {
		delete _lastKeyReleasedEvt;
		_lastKeyReleasedEvt = NULL;
	}
	if (_lastKeyPressedEvt != NULL) {
		delete _lastKeyPressedEvt;
		_lastKeyPressedEvt = NULL;
	}
}

// Methods used for polling input
bool InputManager::isKeyDown(OIS::KeyCode kc) 
{
	return _mKeyboard ? (_mKeyboard->isKeyDown(kc)) : false;
}

bool InputManager::isKeyUp(OIS::KeyCode kc) 
{
	return _mKeyboard ? (!isKeyDown(kc)) : false;
}

bool InputManager::isKeyPressed(OIS::KeyCode kc)
{
	return _lastKeyPressedEvt ? (_lastKeyPressedEvt->key == kc) : false;
}

bool InputManager::isKeyReleased(OIS::KeyCode kc)
{
	return _lastKeyReleasedEvt ? (_lastKeyReleasedEvt->key == kc) : false;
}

bool InputManager::isMouseDown(OIS::MouseButtonID button) 
{
	const OIS::MouseState &ms = _mMouse->getMouseState();
	return ms.buttonDown(button);
}

bool InputManager::isMouseUp(OIS::MouseButtonID button) 
{
	return !isMouseDown(button);
}

bool InputManager::isMouseLeftClicked()
{
	return isMouseDown(OIS::MouseButtonID::MB_Left);
}

bool InputManager::isMouseRightClicked()
{
	return isMouseDown(OIS::MouseButtonID::MB_Right);
}


OIS::KeyEvent* InputManager::getKeyPressedEvent() {
	return _lastKeyPressedEvt;
}

OIS::KeyEvent* InputManager::getKeyReleasedEvent() {
	return _lastKeyReleasedEvt;
}

OIS::MouseEvent* InputManager::getMouseMovedEvent() {
	return _lastMouseMovedEvt;
}

OIS::MouseEvent* InputManager::getMousePressedEvent() {
	return _lastMousePressedEvt;
}

OIS::MouseEvent* InputManager::getMouseReleasedEvent() {
	return _lastMouseReleasedEvt;
}

OIS::Mouse* InputManager::getMouse() {
	return _mMouse;
}

OIS::Keyboard* InputManager::getKeyboard() {
	return _mKeyboard;
}

// Input callbacks
bool InputManager::keyPressed(const OIS::KeyEvent &arg) {
	_lastKeyPressedEvt = new OIS::KeyEvent(arg);
	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &arg) {
	_lastKeyReleasedEvt = new OIS::KeyEvent(arg);
	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &arg) {
	_lastMouseMovedEvt = new OIS::MouseEvent(arg);
	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_lastMousePressedEvt = new OIS::MouseEvent(arg);
	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	_lastMouseReleasedEvt = new OIS::MouseEvent(arg);
	return true;
}

// Event handlers for window events
void InputManager::windowResized(Ogre::RenderWindow* rw) 
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = _mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void InputManager::windowClosed(Ogre::RenderWindow* rw) 
{
	Ogre::WindowEventUtilities::removeWindowEventListener(rw, this);
    //Only close for window that created OIS (the main window in these demos)
    if(_mInputManager)
    {
        _mInputManager->destroyInputObject(_mMouse);
        _mInputManager->destroyInputObject(_mKeyboard);

        OIS::InputManager::destroyInputSystem(_mInputManager);
        _mInputManager = 0;
    }
}
