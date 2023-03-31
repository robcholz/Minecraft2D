//
// Created by robcholz on 3/31/23.
//

#ifndef RUNCRAFT_SCREENMANAGER_HPP
#define RUNCRAFT_SCREENMANAGER_HPP

#include <list>
#include "Screen.hpp"

class ScreenManager {
private:
	std::list<Screen *> screenList;
public:
	ScreenManager() = default;

	ScreenManager &addScreen(Screen *screen) {
		screenList.push_back(screen);
		return *this;
	}

	void setEntryScreen(Screen *entryScreen) {entryScreen->enable(true); }
};

#endif //RUNCRAFT_SCREENMANAGER_HPP
