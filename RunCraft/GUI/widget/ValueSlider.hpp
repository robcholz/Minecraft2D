//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_VALUESLIDER_HPP
#define RUNCRAFT_VALUESLIDER_HPP

#include "Slider.hpp"

class ValueSlider : public Slider {
public:
	explicit ValueSlider(const std::string &words, int width = 200, int height = 20, bool visible = true,
	                     int x = 0, int y = 0)
			: Slider(words, width, height, visible, x, y) {
		widgetCurrentPtr->setPosition((float) sliderOutlineBound.upper, (float) sliderOutline.y);
	}

	explicit ValueSlider(const std::string &words, int defaultValue, int width = 200, int height = 20, bool visible = true,
	                     int x = 0, int y = 0)
			: Slider(words, width, height, visible, x, y) {
		sliderValue = defaultValue;
		message.clear();
		message.setMessage(title + ": " + std::to_string(getValue()) + "%");
		widgetCurrentPtr->setPosition(
				(float) sliderOutlineBound.lower +((float) defaultValue / 100) * (float) (sliderOutlineBound.upper - sliderOutlineBound.lower),
				(float) sliderOutline.y);
	}

	void action() override {
		message.clear();
		sliderValue = (ButtonValue)(
				((float) (sliderOutline.x - widgetOutline.x) / (float) (widgetOutline.width - sliderOutline.width - 8)) * 100.0f);
		if (getValue() != 0)
			message.setMessage(title + ": " + std::to_string(getValue()) + "%");
		else message.setMessage(title + ": OFF");
	}
};

#endif //RUNCRAFT_VALUESLIDER_HPP
