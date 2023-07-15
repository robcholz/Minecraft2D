//
// Created by robcholz on 6/30/23.
//

#ifndef RUNCRAFT_RUNCRAFTCLIENTACCESS_HPP
#define RUNCRAFT_RUNCRAFTCLIENTACCESS_HPP


class SoundManager;

class SceneManager;

namespace input::mouse {
	class Mouse;
}
namespace input::keyboard {
	class Keyboard;
}

class RuncraftClientAccess {
public:
	virtual SoundManager* getSoundManager() = 0;

	virtual SceneManager* getSceneManager() = 0;

	virtual input::mouse::Mouse* getMouse() = 0;

	virtual input::keyboard::Keyboard* getKeyboard() = 0;
};

#endif //RUNCRAFT_RUNCRAFTCLIENTACCESS_HPP
