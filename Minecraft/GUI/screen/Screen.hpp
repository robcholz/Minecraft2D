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

enum ScreenEvent:unsigned char {
	SCREEN_TRANSFER,
	SCREEN_NORMAL
};

class Screen {
private:
	Background *background = nullptr;
	std::list<Widget *> widgetsList;
	InputState *inputStatePtr = nullptr;

	Screen *callBackScreenPtr = nullptr;
	Button *callBackButtonPtr = nullptr;

	AudioPlayer audioPlayer;
	ScreenEvent screenEvent;
public:
	explicit Screen(Background *background) {
		this->background = background;
	}

	~Screen() = default;

	Screen &addWidget(Widget *widget) {
		widgetsList.push_back(widget);
		return *this;
	}

	ScreenEvent getScreenEvent() const { return screenEvent; }

	Screen &setCallbackScreen(Screen *callBackScreen, Button *callBackButton) {
		callBackScreenPtr = callBackScreen;
		callBackButtonPtr = callBackButton;
		return *this;
	}

	Screen *getCallBackScreen() { return callBackScreenPtr; }

	void listen(InputState *inputState) { inputStatePtr = inputState; }

	void render() {
		background->render();
		for (auto *widget_obj: widgetsList) {
			widget_obj->listen(inputStatePtr->mousePosition, inputStatePtr->isPressed);
			widget_obj->render();
			if (widget_obj->isClicked() && widget_obj->stateChanged()) {
				audioPlayer.play();
				if (callBackButtonPtr->isPressed()) {
					widget_obj->action();
					callBackButtonPtr->setState(false);
					screenEvent=SCREEN_TRANSFER;
					return;
				}
				widget_obj->action();
			}
		}
		screenEvent=SCREEN_NORMAL;
	}
};

#endif //RUNCRAFT_SCREEN_HPP
