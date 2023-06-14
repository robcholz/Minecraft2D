//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_SOUNDSLIDERWIDGET_HPP
#define RUNCRAFT_SOUNDSLIDERWIDGET_HPP

#include "SliderWidget.hpp"

#include <utility>

class SoundSliderWidget : public SliderWidget {
private:
	internal_data::SoundVolume* controlledValuePtr = nullptr;
public:
	explicit SoundSliderWidget(const std::string& id, int width = 200, int height = 20, bool visible = true,
	                           int x = 0, int y = 0) : SliderWidget(id, width, height, visible, x, y) {
		widgetSprite.setPosition((float) widgetOutlineBound.upper, (float) widgetOutline.y);
	}

	explicit SoundSliderWidget(const std::string& words, int defaultValue, int width = 200, int height = 20, bool visible = true,
	                           int x = 0, int y = 0) : SliderWidget(words, width, height, visible, x, y) {
		sliderValue = defaultValue;
		message.clear();
		message.setMessage(title + ": " + std::to_string(getValue()) + "%");
		widgetSprite.setPosition((float) widgetOutlineBound.lower
		                         + ((float) defaultValue / 100) * (float) (widgetOutlineBound.upper - widgetOutlineBound.lower), (float) widgetOutline.y);
	}

	SoundSliderWidget& varToChangeWhenMoved(internal_data::SoundVolume* controlledValue = nullptr) {
		this->controlledValuePtr = controlledValue;
		return *this;
	}

	void executeCallbackFunc() override {
		Widget::executeCallbackFunc();
		message.clear();
		sliderValue = (ButtonValue)(
				((float) (widgetOutline.x - widgetBackgroundOutline.x) / (float) (widgetBackgroundOutline.width - widgetOutline.width - 8)) * 100.0f);
		*controlledValuePtr = getValue();
		auto msg = title + ": OFF";
		if (getValue() != 0)
			msg = title + ": " + std::to_string(getValue()) + "%";
		message.setMessage(msg);
	}
};

#endif //RUNCRAFT_SOUNDSLIDERWIDGET_HPP
