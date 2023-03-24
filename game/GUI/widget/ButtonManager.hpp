//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_BUTTONMANAGER_HPP
#define RUNCRAFT_BUTTONMANAGER_HPP

#include <vector>
#include "Button.hpp"

class ButtonManager {
private:
	std::vector<Button *> buttonList;
	AudioPlayer audioPlayer;
public:
	ButtonManager() = default;

	ButtonManager &addButton(Button *button) {
		buttonList.push_back(button);
		return *this;
	}

	~ButtonManager() = default;

	void listen(sf::Vector2i mousePosition, bool isPressed) {
		for (auto *button_obj: buttonList) {
			button_obj->listen(mousePosition, isPressed);
			if (button_obj->pressed() && button_obj->stateChanged()) { audioPlayer.play(); }
			button_obj->render();
		}
	}
};

#endif //RUNCRAFT_BUTTONMANAGER_HPP
