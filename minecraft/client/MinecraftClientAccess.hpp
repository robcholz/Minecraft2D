//
// Created by robcholz on 6/30/23.
//

#ifndef MINECRAFT_MINECRAFTCLIENTACCESS_HPP
#define MINECRAFT_MINECRAFTCLIENTACCESS_HPP


class SoundManager;

class SceneManager;

namespace input::mouse {
	class Mouse;
}
namespace input::keyboard {
	class Keyboard;
}

class MinecraftClientAccess {
public:
	virtual SoundManager* getSoundManager() = 0;

	virtual SceneManager* getSceneManager() = 0;

	virtual input::mouse::Mouse* getMouse() = 0;

	virtual input::keyboard::Keyboard* getKeyboard() = 0;
};

#endif //MINECRAFT_MINECRAFTCLIENTACCESS_HPP
