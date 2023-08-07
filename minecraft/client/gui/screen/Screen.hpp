//
// Created by robcholz on 3/25/23.
//
#pragma once

#ifndef MINECRAFT_SCREEN_HPP
#define MINECRAFT_SCREEN_HPP


#include <memory>
#include <map>
#include "sound/SoundEvents.hpp"


class Screen {
public:
	explicit Screen(MinecraftClientAccess* minecraftClientAccess, Background* background) {
		this->background = background;
		this->minecraftClientAccess = minecraftClientAccess;
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

	void update(){
		for (const auto widget_obj: widgetsList) {
			widget_obj->update();
			if (widget_obj->isClicked() && widget_obj->isFocused()) {
				this->minecraftClientAccess->getSoundManager()->addSound(SoundEvents::getInstance().CLICK_SOUND_GUI);
				widget_obj->executeCallbackFunc();
				if (callbackScreenMap.contains(widget_obj)) {
					responseCallBackScreenPtr = callbackScreenMap[widget_obj];
					return;
				}
			} else
				responseCallBackScreenPtr = nullptr;
		}
	}

	void render() {
		background->render();
		for (const auto widget_obj: widgetsList) {
			widget_obj->render();
		}
	}

private:
	MinecraftClientAccess* minecraftClientAccess= nullptr;
	Screen* responseCallBackScreenPtr = nullptr;
	Background* background = nullptr;
	std::list<Widget*> widgetsList;
	std::map<Widget*, Screen*> callbackScreenMap;
};

#endif //MINECRAFT_SCREEN_HPP
