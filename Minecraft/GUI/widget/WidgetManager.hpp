//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_WIDGETMANAGER_HPP
#define RUNCRAFT_WIDGETMANAGER_HPP

#pragma once

#include <list>
#include "Sound/Audio.hpp"
#include "GameInfo.hpp"

class WidgetManager {
private:
	std::list<Widget *> widgetsList;
	AudioPlayer audioPlayer;
public:
	WidgetManager() = default;

	WidgetManager &addWidget(Widget *widget) {
		widgetsList.push_back(widget);
		return *this;
	}

	~WidgetManager() = default;

	void listen(InputState *inputState) {
		for (auto *widget_obj: widgetsList) {
			widget_obj->listen(inputState->mousePosition, inputState->isPressed);
			widget_obj->render();
			if (widget_obj->isClicked() && widget_obj->stateChanged()) {
				audioPlayer.play();
				widget_obj->action();
			}
		}
	}
};

#endif //RUNCRAFT_WIDGETMANAGER_HPP
