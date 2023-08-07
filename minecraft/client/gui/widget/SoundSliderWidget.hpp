//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_SOUNDSLIDERWIDGET_HPP
#define MINECRAFT_SOUNDSLIDERWIDGET_HPP

#include <utility>
#include "SliderWidget.hpp"

class SoundSliderWidget : public SliderWidget {
public:
	explicit SoundSliderWidget(const std::string& id, int width = 200, int height = 20, bool visible = true,
	                           int x = 0, int y = 0) : SliderWidget(id, width, height, visible, x, y) {
		widgetSprite.setPosition((float) widgetOutlineBound.upper, (float) widgetOutline.y);
	}

	explicit SoundSliderWidget(const std::string& words, int defaultValue, int width = 200, int height = 20, bool visible = true,
	                           int x = 0, int y = 0) : SliderWidget(words, width, height, visible, x, y) {
		sliderValue = defaultValue;
		message.clear();
		message.setMessage(getDisplayContent(title, getValue()));
		widgetSprite.setPosition((float) widgetOutlineBound.lower
		                         + ((float) defaultValue / 100) * (float) (widgetOutlineBound.upper - widgetOutlineBound.lower), (float) widgetOutline.y);
	}

	~SoundSliderWidget() override = default;

	SoundSliderWidget& varToChangeWhenMoved(SoundManager::SoundVolume* controlledValue = nullptr) {
		this->controlledValuePtr = controlledValue;
		return *this;
	}

	void executeCallbackFunc() override {
		Widget::executeCallbackFunc();
		message.clear();
		sliderValue = (ButtonValue)(((float) (widgetOutline.x - widgetBackgroundOutline.x)
		                             / (float) (widgetBackgroundOutline.width - widgetOutline.width - 8)) * 100.0f);
		if (controlledValuePtr)
			*controlledValuePtr = getValue();
		auto msg = title + L": OFF";
		if (getValue() != 0)
			msg = getDisplayContent(title, getValue());
		message.setMessage(msg);
	}

private:
	SoundManager::SoundVolume* controlledValuePtr = nullptr;
};

#endif //MINECRAFT_SOUNDSLIDERWIDGET_HPP
