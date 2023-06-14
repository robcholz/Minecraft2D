//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SCREEN_HPP
#define RUNCRAFT_SCREEN_HPP

#pragma once

#include <memory>
#include "client/GameInfo.hpp"
#include "sound/Audio.hpp"


class Screen {
private:
	Background* background = nullptr;
	std::list<Widget*> widgetsList;

	std::map<Widget*, Screen*> callbackScreenMap;
	Screen* responseCallBackScreenPtr = nullptr;

	AudioPlayer audioPlayer;
public:
	explicit Screen(Background* background) {
		this->background = background;
	}

	~Screen() = default;

	Screen& addWidget(Widget* widget) {
		widgetsList.push_back(widget);
		return *this;
	}

	Screen& addCallbackScreen(Screen* callBackScreen, Widget* callBackButton) {
		callbackScreenMap.insert({callBackButton, callBackScreen});
		return *this;
	}

	Screen* getResponseCallbackScreen() { return responseCallBackScreenPtr; }

	void render() {
		background->render();
		for (auto widget_obj: widgetsList) {
			widget_obj->update();
			if (widget_obj->isClicked() && widget_obj->isFocused()) {
				audioPlayer.play();
				widget_obj->executeCallbackFunc();
				if (callbackScreenMap.contains(widget_obj)) {
					responseCallBackScreenPtr = callbackScreenMap[widget_obj];
					return;
				}
			} else responseCallBackScreenPtr = nullptr;
			widget_obj->render();
		}
	}
};

#endif //RUNCRAFT_SCREEN_HPP
