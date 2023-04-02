//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_VALUESLIDER_HPP
#define RUNCRAFT_VALUESLIDER_HPP

#include "Slider.hpp"

#include <utility>

class ValueSlider : public Slider {
private:
	ActionWhenActivated execFuncPtr = nullptr;
	game_data::SoundVolume *controlledValuePtr = nullptr;
public:
	explicit ValueSlider(const std::string &words, int width = 200, int height = 20, bool visible = true,
	                     int x = 0, int y = 0)
			: Slider(words, width, height, visible, x, y) {
		widgetCurrentSprite.setPosition((float) sliderOutlineBound.upper, (float) widgetOutline.y);
	}

	explicit ValueSlider(const std::string &words, int defaultValue, int width = 200, int height = 20, bool visible = true,
	                     int x = 0, int y = 0)
			: Slider(words, width, height, visible, x, y) {
		sliderValue = defaultValue;
		message.clear();
		message.setMessage(title + ": " + std::to_string(getValue()) + "%");
		widgetCurrentSprite.setPosition(
				(float) sliderOutlineBound.lower +
				((float) defaultValue / 100) * (float) (sliderOutlineBound.upper - sliderOutlineBound.lower),
				(float) widgetOutline.y);
	}

	ValueSlider &varToChangeWhenMoved(game_data::SoundVolume *controlledValue = nullptr) { this->controlledValuePtr = controlledValue; return *this;}

	ValueSlider &actionsToExecWhenMoved(ActionWhenActivated execFunc) { this->execFuncPtr = std::move(execFunc); return *this;}

	void action() override {
		message.clear();
		sliderValue = (ButtonValue)(
				((float) (widgetOutline.x - sliderBackgroundOutline.x) / (float) (sliderBackgroundOutline.width - widgetOutline.width - 8)) * 100.0f);
		*controlledValuePtr = sliderValue;
		if (getValue() != 0)
			message.setMessage(title + ": " + std::to_string(getValue()) + "%");
		else message.setMessage(title + ": OFF");
		if (execFuncPtr != nullptr)
			execFuncPtr();
	}
};

#endif //RUNCRAFT_VALUESLIDER_HPP
