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
		for (auto *widget_obj: widgetsList) {
			widget_obj->listen(mousePosition, isPressed);
			if (widget_obj->activated() && widget_obj->stateChanged()) { audioPlayer.play(); }
			widget_obj->render();
		}
	}
};

#endif //RUNCRAFT_WIDGETMANAGER_HPP
