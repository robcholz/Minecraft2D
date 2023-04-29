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
	Background *background = nullptr;
	std::list<Widget *> widgetsList;
	external_data::PeripheralState *inputStatePtr = nullptr;

	std::map<ButtonWidget *, Screen *> callbackScreenMap;
	Screen *responseCallBackScreenPtr = nullptr;

	AudioPlayer audioPlayer;
public:
	explicit Screen(Background *background) {
		this->background = background;
	}

	~Screen() = default;

	Screen &addWidget(Widget *widget) {
		widgetsList.push_back(widget);
		return *this;
	}

	Screen &addCallbackScreen(Screen *callBackScreen, ButtonWidget *callBackButton) {
		callbackScreenMap.insert({callBackButton, callBackScreen});
		return *this;
	}

	Screen *getResponseCallbackScreen() {return responseCallBackScreenPtr;}

	void listen(external_data::PeripheralState *inputState) { inputStatePtr = inputState; }

	void render() {
		background->render();
		for (auto *widget_obj: widgetsList) {
			widget_obj->listen(inputStatePtr->mouseRelativeToGameWindowPos, inputStatePtr->isButtonPressedLeft);
			if (widget_obj->isClicked()) {
				audioPlayer.play();
				for (auto & screen_map_obj : callbackScreenMap) {
					if(screen_map_obj.first->isClicked()){
						widget_obj->action();
						screen_map_obj.first->updateState(false);
						responseCallBackScreenPtr=screen_map_obj.second;
						return;
					}
				}
				widget_obj->action();
			} else responseCallBackScreenPtr= nullptr;
			widget_obj->render();
		}
	}
};

#endif //RUNCRAFT_SCREEN_HPP
