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
#include "util/Math_Helper.hpp"

class Widget : public GUI {
protected:
	sf::Texture sliderBackgroundNormal;
	sf::Texture widgetActivated;
	sf::Sprite *widgetCurrentPtr = new sf::Sprite;
	std::shared_ptr<sf::Vector2i> *widgetSize;
	Areai widgetOutline{};

	inline static sf::Font font;
	std::string widgetAssetPath = guiFilePath + "widgets.png";
	std::string fontAssetPath = fontFilePath + "runcraft.ttf";

	bool visible = true;
	bool lastState = false, stateChange = false, clickState = false, pressed = false;

	void setClicked(bool clicked) { clickState = clicked; }

public:
	explicit Widget() {
		font.loadFromFile(fontAssetPath);
		widgetSize = nullptr;
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

	bool static checkVectorBoundary(sf::Vector2i vector, sf::IntRect area) {
		return ((vector.x > area.left && vector.x < area.left + area.width) &&
		        (vector.y > area.top - area.height && vector.y < area.height));
	}

	bool static checkVectorBoundary(int x, int y, int left, int top, int width, int height) {
		return ((x > left && x < left + width) && (y > top && y < top + height));
	}

	void setVisibility(bool visibility) { visible = visibility; }

	bool getVisiailibty() const { return visible; }

	virtual void action() = 0;

	bool activated() const { return lastState; }

	bool isClicked() const { return clickState; }

	bool isPressed() const { return pressed; }

	virtual void listen(sf::Vector2i mousePosition, bool isPressed) = 0;

	bool stateChanged() const { return stateChange; }
};

#endif //RUNCRAFT_WIDGET_HPP
