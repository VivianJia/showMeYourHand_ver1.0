#include "glListener.h"

using namespace Leap;

void glListener::onInit(const Controller&controller){
	/*cout << "initialized" << endl;*/
}

//callback function
void glListener::onFrame(const Controller& controller){
	//Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	if(!frame.hands().empty()) {
		//get the first hand
		const Hand hand = frame.hands()[0];
		
		//check if the hand has any fingers
		const FingerList fingers = hand.fingers();
		if (!fingers.empty()) {
			/*Vector pos[100];
			Vector velocity[100];
			for(int i = 0; i < fingers.count(); i++) {
				pos[i] = fingers[i].tipPosition();
				velocity[i] = fingers[i].tipVelocity();
			}*/

			for(int i = 0; i < fingers.count(); i++){
				
			}
		}
	}
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