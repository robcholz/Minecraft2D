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
#include "GUI/Style/GUIStyle.hpp"

class Widget : public GUI {
protected:
	typedef std::function<void()> ActionWhenActivated;

	sf::Texture sliderBackgroundNormal;
	sf::Texture widgetActivated;
	sf::Sprite widgetCurrentSprite;
	std::shared_ptr<sf::Vector2i> *widgetSize;
	Areai widgetOutline{};

	std::string widgetAssetPath = guiFilePath + "widgets.png";

	bool visible = true;
	bool lastState = false, stateChange = false, clickState = false, pressed = false;

	void setClicked(bool clicked) { clickState = clicked; }
public:
	explicit Widget() {
		widgetSize = nullptr;
	}

	virtual void setState(bool state) {
		pressed = state;
		setClicked(!lastState && state);
		if (lastState != state) {
			stateChange = true;
			lastState = state;
			if (lastState) {
				widgetCurrentSprite.setTexture(widgetActivated);
				return;
			} else {
				widgetCurrentSprite.setTexture(sliderBackgroundNormal);
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

	void render() override {}

	bool stateChanged() const { return stateChange; }
};

#endif //RUNCRAFT_WIDGET_HPP
