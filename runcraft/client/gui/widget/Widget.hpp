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
#include "util/MathHelper.hpp"

class Widget : public GUI {
protected:
	typedef std::function<void()> ActionWhenActivated;

	sf::Texture widgetNormalTexture;
	sf::Texture widgetActivatedTexture;
	sf::Sprite widgetCurrentSprite;
	std::shared_ptr<sf::Vector2i> *widgetSize;
	Areai widgetOutline{};

	std::string widgetAssetPath = guiFilePath + "widgets.png";

	bool visible = true;
	bool lastState = false, clickState = false, pressed = false;
	std::string id;
public:
	explicit Widget(const std::string &id) {
		this->id = id;
		widgetSize = nullptr;
	}

	virtual void updateState(bool state) {
		pressed = state;
		clickState = lastState != state && state;
		lastState = state;
	}

	void setVisibility(bool visibility) { visible = visibility; }

	bool getVisibility() const { return visible; }

	virtual void action() = 0;

	bool isClicked() const { return clickState; }

	bool isPressed() const { return pressed; }

	virtual void listen(sf::Vector2i mousePosition, bool isPressed) {
		if (getVisibility()) {
			if (checkVectorBoundary(mousePosition, widgetOutline)) {
				updateState(isPressed);
				widgetCurrentSprite.setTexture(widgetActivatedTexture);
			} else {
				widgetCurrentSprite.setTexture(widgetNormalTexture);
			}
		}
	}

	void render() override {}
};

#endif //RUNCRAFT_WIDGET_HPP
