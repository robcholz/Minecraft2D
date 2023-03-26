//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_WIDGETMANAGER_HPP
#define RUNCRAFT_WIDGETMANAGER_HPP

#pragma once

#include <vector>
#include "Button.hpp"

class WidgetManager {
private:
	std::vector<Widget *> widgetsList;
	AudioPlayer audioPlayer;
public:
	WidgetManager() = default;

	WidgetManager &addWidget(Widget *widget) {
		widgetsList.push_back(widget);
		return *this;
	}

	~WidgetManager() = default;

	void listen(sf::Vector2i mousePosition, bool isPressed) {
		for (auto *button_obj: widgetsList) {
			button_obj->listen(mousePosition, isPressed);
			if (button_obj->pressed() && button_obj->stateChanged()) { audioPlayer.play(); }
			button_obj->render();
		}
	}
};

#endif //RUNCRAFT_WIDGETMANAGER_HPP
