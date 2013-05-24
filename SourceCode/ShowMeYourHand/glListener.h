#ifndef GLLISTENER_H
#define GLLISTENER_H

#include "Leap.h"


using namespace Leap;

class glListener :public Listener{
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
};

#endif