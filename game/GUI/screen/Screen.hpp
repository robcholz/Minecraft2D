//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SCREEN_HPP
#define RUNCRAFT_SCREEN_HPP

#pragma once

#include "../widget/WidgetManager.hpp"

class Screen {
private:
	Background *background = new Background("options_background.png");
public:
	explicit Screen() {
		background->composeToScreen();
	}

	Screen &addWidgetManager(WidgetManager *widgetManager) {
		return *this;
	}
};

#endif //RUNCRAFT_SCREEN_HPP
