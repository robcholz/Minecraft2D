//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_SOUNDSLIDERWIDGET_HPP
#define RUNCRAFT_SOUNDSLIDERWIDGET_HPP

#include "SliderWidget.hpp"

#include <utility>

class SoundSliderWidget : public SliderWidget {
private:
	ActionWhenActivated execFuncPtr = nullptr;
	internal_data::SoundVolume *controlledValuePtr = nullptr;
public:
	explicit SoundSliderWidget(const std::string &id, int width = 200, int height = 20, bool visible = true,
	                           int x = 0, int y = 0)
			: SliderWidget(id, width, height, visible, x, y) {
		widgetCurrentSprite.setPosition((float) sliderOutlineBound.upper, (float) widgetOutline.y);
	}

	explicit SoundSliderWidget(const std::string &words, int defaultValue, int width = 200, int height = 20, bool visible = true,
	                           int x = 0, int y = 0)
			: SliderWidget(words, width, height, visible, x, y) {
		sliderValue = defaultValue;
		message.clear();
		message.setMessage(title + ": " + std::to_string(getValue()) + "%");
		widgetCurrentSprite.setPosition(
				(float) sliderOutlineBound.lower +
				((float) defaultValue / 100) * (float) (sliderOutlineBound.upper - sliderOutlineBound.lower),
				(float) widgetOutline.y);
	}

	SoundSliderWidget &varToChangeWhenMoved(internal_data::SoundVolume *controlledValue = nullptr) { this->controlledValuePtr = controlledValue; return *this;}

	SoundSliderWidget &actionsToExecWhenMoved(const ActionWhenActivated &execFunc) { this->execFuncPtr = execFunc; return *this;}

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

#endif //RUNCRAFT_SOUNDSLIDERWIDGET_HPP
