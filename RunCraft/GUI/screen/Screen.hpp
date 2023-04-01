//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SCREEN_HPP
#define RUNCRAFT_SCREEN_HPP

#pragma once

#include <memory>
#include "ErrorHandling/ErrorHandling.hpp"
#include "GameInfo.hpp"
#include "GUI/widget/Button.hpp"


class Screen {
private:
	Background *background = nullptr;
	std::list<Widget *> widgetsList;
	InputState *inputStatePtr = nullptr;

	std::map<Button *, Screen *> callbackScreenMap;
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

	//ScreenEvent getScreenEvent() const { return screenEvent; }

	Screen &addCallbackScreen(Screen *callBackScreen, Button *callBackButton) {
		callbackScreenMap.insert({callBackButton, callBackScreen});
		return *this;
	}

	Screen *getResponseCallbackScreen() {return responseCallBackScreenPtr;}

	void listen(InputState *inputState) { inputStatePtr = inputState; }

	void render() {
		background->render();
		for (auto *widget_obj: widgetsList) {
			widget_obj->listen(inputStatePtr->mousePosition, inputStatePtr->isPressed);
			if (widget_obj->isClicked() && widget_obj->stateChanged()) {
				audioPlayer.play();
				for (auto & screen_map_obj : callbackScreenMap) {
					if(screen_map_obj.first->isPressed()){
						widget_obj->action();
						screen_map_obj.first->setState(false);
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
