//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SCREEN_HPP
#define RUNCRAFT_SCREEN_HPP

#pragma once

#include <memory>
#include <map>
#include "sound/SoundEvents.hpp"


class Screen {
public:
	explicit Screen(RuncraftClientAccess* runcraftClientAccess, Background* background) {
		this->background = background;
		this->runcraftClientAccess = runcraftClientAccess;
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
				this->runcraftClientAccess->getSoundManager()->addSound(SoundEvents::getInstance().GUI_CLICK_SOUND_1);
				widget_obj->executeCallbackFunc();
				if (callbackScreenMap.contains(widget_obj)) {
					responseCallBackScreenPtr = callbackScreenMap[widget_obj];
					return;
				}
			} else responseCallBackScreenPtr = nullptr;
			widget_obj->render();
		}
	}

private:
	Background* background = nullptr;
	std::list<Widget*> widgetsList;
	std::map<Widget*, Screen*> callbackScreenMap;
	Screen* responseCallBackScreenPtr = nullptr;
	RuncraftClientAccess* runcraftClientAccess= nullptr;
};

#endif //RUNCRAFT_SCREEN_HPP
