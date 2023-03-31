//
// Created by robcholz on 3/25/23.
//

#ifndef RUNCRAFT_SCREEN_HPP
#define RUNCRAFT_SCREEN_HPP

#pragma once

#include <memory>
#include "ErrorHandling/ErrorHandling.hpp"
#include "GameInfo.hpp"
#include "GUI/widget/WidgetManager.hpp"
#include "GUI/widget/Button.hpp"

class Screen : public GUI {
private:
	Background *background = nullptr;
	std::shared_ptr<WidgetManager> *widgetManager = nullptr;
	InputState *inputState = nullptr;

	bool isEnabled = false;
public:
	explicit Screen(Background *background) {
		this->background = background;
		widgetManager = new std::shared_ptr<WidgetManager>(new WidgetManager);
	}

	explicit Screen(Background *background, WidgetManager *widgetManager) {
		this->background = background;
		this->widgetManager = reinterpret_cast<std::shared_ptr<WidgetManager> *>(widgetManager);
	}

	void enable(bool enable) {isEnabled = enable;}

	void setInputStatePtr(InputState *pInputState) { this->inputState = pInputState; }

	void setCallbackScreenInstance(){

	}

	[[nodiscard]] std::shared_ptr<WidgetManager> *getWidgetManager() const {
		if (widgetManager != nullptr)return widgetManager;
		else throw NullPtrException();
	}

	~Screen() = default;

	void render() override {
		if(isEnabled){
			background->render();
			widgetManager->get()->listen(inputState);
		}
	}
};

#endif //RUNCRAFT_SCREEN_HPP
