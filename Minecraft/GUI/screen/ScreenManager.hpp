//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_SCREENMANAGER_HPP
#define RUNCRAFT_SCREENMANAGER_HPP

#include <vector>
#include "Screen.hpp"

class ScreenManager : public GUI {
private:
	std::vector<Screen *> screenList;
	Screen *currentRenderingScreen = nullptr;
	InputState *inputState = nullptr;
public:
	ScreenManager() = default;

	ScreenManager &addScreen(Screen *screen) {
		screenList.push_back(screen);
		return *this;
	}

	void setEntryScreen(Screen *entryScreen) {
		currentRenderingScreen = entryScreen;
	}

	void setInputStatePtr(InputState *pInputState) { this->inputState = pInputState; }

	void render() override {
		currentRenderingScreen->listen(inputState);
		currentRenderingScreen->render();
		if (currentRenderingScreen->getScreenEvent() == ScreenEvent::SCREEN_TRANSFER) {
			currentRenderingScreen = currentRenderingScreen->getCallBackScreen();
		}
	}

};

#endif //RUNCRAFT_SCREENMANAGER_HPP
