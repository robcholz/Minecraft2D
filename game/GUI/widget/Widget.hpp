//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_WIDGET_HPP
#define RUNCRAFT_WIDGET_HPP

#pragma once

#include "../GUI.hpp"
#include "../text/RichText.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>

class Widget : public GUI {
protected:
	sf::Texture widgetNormal;
	sf::Texture widgetActivated;
	sf::Sprite *widgetCurrentPtr = new sf::Sprite;
	std::shared_ptr<sf::Vector2f> *widgetSize;
	std::shared_ptr<sf::Vector2i> *widgetOutlinePosition;

	inline static sf::Font font;
	std::string widgetAssetPath = guiFilePath + "widgets.png";
	std::string fontAssetPath = fontFilePath + "runcraft.ttf";

	bool visible = true;
	bool lastState = false, stateChange = false;
public:
	explicit Widget() {
		font.loadFromFile(fontAssetPath);
		widgetSize = nullptr;
		widgetOutlinePosition = nullptr;
	}

	void setState(bool state) {
		if (lastState != state) {
			stateChange = true;
			lastState = state;
			if (lastState) {
				widgetCurrentPtr->setTexture(widgetActivated);
				return;
			} else { widgetCurrentPtr->setTexture(widgetNormal); }
		} else stateChange = false;
	}

	[[nodiscard]] Vector2D<int> *getSize() const { return reinterpret_cast<Vector2D<int> *>(widgetSize->get()); }

	void setVisibility(bool visibility) {
		visible = visibility;
	}

	bool pressed() const { return lastState; }

	bool stateChanged() const { return stateChange; }

	virtual void listen(sf::Vector2i mousePosition, bool isPressed) = 0;
};

#endif //RUNCRAFT_WIDGET_HPP
