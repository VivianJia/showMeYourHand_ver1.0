#include "glListener.h"

using namespace Leap;

void glListener::onInit(const Controller&controller){
	/*cout << "initialized" << endl;*/
}

void glListener::onConnect(const Controller& controller) {
	//std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void glListener::onDisconnect(const Controller& controller) {
	//Note: not dispatched when running in a debugger.
	//std::cout << "Disconnected" << std::endl;
}

void glListener::onExit(const Controller& controller) {
	//std::cout << "Exited" << std::endl;
}

void glListener::onFocusGained(const Controller& controller) {
	//std::cout << "Focus Gained" << std::endl;
}

void glListener::onFocusLost(const Controller& controller) {
	//std::cout << "Focus Lost" << std::endl;
}