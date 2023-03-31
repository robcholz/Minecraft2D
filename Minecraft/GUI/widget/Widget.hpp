//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_WIDGET_HPP
#define RUNCRAFT_WIDGET_HPP

#pragma once

class Screen; /*fuck circular dependencies*/

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>

class Widget : public GUI {
protected:
	sf::Texture sliderBackgroundNormal;
	sf::Texture widgetActivated;
	sf::Sprite *widgetCurrentPtr = new sf::Sprite;
	std::shared_ptr<sf::Vector2f> *widgetSize;
	std::shared_ptr<sf::Vector2i> *widgetOutlinePosition;

	inline static sf::Font font;
	std::string widgetAssetPath = guiFilePath + "widgets.png";
	std::string fontAssetPath = fontFilePath + "runcraft.ttf";

	bool visible = true;
	bool lastState = false, stateChange = false, clickState = false;
	bool pressed = false;

	void setClicked(bool clicked) { clickState = clicked; }

	bool activated() const { return lastState; }

public:
	explicit Widget() {
		font.loadFromFile(fontAssetPath);
		widgetSize = nullptr;
		widgetOutlinePosition = nullptr;
	}

	virtual void setState(bool state) {
		pressed = state;
		setClicked(!lastState && state);
		if (lastState != state) {
			stateChange = true;
			lastState = state;
			if (lastState) {
				widgetCurrentPtr->setTexture(widgetActivated);
				return;
			} else {
				widgetCurrentPtr->setTexture(sliderBackgroundNormal);
			}
		} else stateChange = false;
	}

	void setVisibility(bool visibility) { visible = visibility; }

	bool getVisiailibty() const { return visible; }

	virtual void action() = 0;

	bool isClicked() const { return clickState; }

	bool isPressed() const { return pressed; }

	virtual void listen(sf::Vector2i mousePosition, bool isPressed) = 0;

	bool stateChanged() const { return stateChange; }
};

#endif //RUNCRAFT_WIDGET_HPP