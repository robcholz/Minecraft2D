//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_SCREENMANAGER_HPP
#define RUNCRAFT_SCREENMANAGER_HPP

#include <vector>
#include "Screen.hpp"

class ScreenManager : public GUI {
private:
	std::list<Screen *> screenList;
	Screen *currentRenderingScreen = nullptr;
	external_data::PeripheralState *inputState = nullptr;
public:
	ScreenManager() = default;

	ScreenManager &addScreen(Screen *screen) {
		screenList.push_back(screen);
		return *this;
	}

	void setEntry(Screen *entryScreen) {
		currentRenderingScreen = entryScreen;
	}

	void setInputStatePtr(external_data::PeripheralState *pInputState) { this->inputState = pInputState; }

	void render() override {
		currentRenderingScreen->listen(inputState);
		currentRenderingScreen->render();
		if (currentRenderingScreen->getResponseCallbackScreen()!= nullptr) {
			currentRenderingScreen = currentRenderingScreen->getResponseCallbackScreen();
		}
	}

};

#endif //RUNCRAFT_SCREENMANAGER_HPP
