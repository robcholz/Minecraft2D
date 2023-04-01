//
// Created by robcholz on 3/8/23.
//

#ifndef RUNCRAFT_BUTTON_HPP
#define RUNCRAFT_BUTTON_HPP

#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include "GameInfo.hpp"
#include "Widget.hpp"
#include "GUI/text/Text.hpp"

class Button : public Widget {
private:
	typedef void (*ActionWhenClicked)(void);

	ActionWhenClicked execFuncPtr = nullptr;

	Text message;

	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 66, 200, 20));
	inline static std::shared_ptr<sf::IntRect> *intRectClicked = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20));
public:
	explicit Button(const std::string &words, int width = 400, int height = 80, bool visible = true, int x = 0, int y = 0) {
		this->visible = visible;
		widgetOutline.x = x;
		widgetOutline.y = y;
		widgetOutline.width = width;
		widgetOutline.height = height;
		widgetSize = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(width, height));

		sliderBackgroundNormal.loadFromFile(widgetAssetPath, **intRectNormal);
		widgetActivated.loadFromFile(widgetAssetPath, **intRectClicked);

		widgetCurrentPtr->setTexture(sliderBackgroundNormal, widgetSize);
		widgetCurrentPtr->setScale((float) width / 200, (float) height / 20);
		widgetCurrentPtr->setPosition((float) widgetOutline.x, (float) widgetOutline.y);

		message.setFont(font).setColor(sf::Color::White).setMessage(words)
				.setPosition((float) widgetOutline.x + (float) widgetOutline.width / 2 - message.getGlobalBounds().width,
				             (float) widgetOutline.y - (float) widgetOutline.height / 8 - 1.0f);
		message.setCharacterSize((int) ((float) widgetOutline.height / 80.0f * 64.0f));
	}

	~Button() {
		delete widgetCurrentPtr;
	}

	void listen(sf::Vector2i mousePos, bool isPressed) override {
		if (visible) {
			if (mousePos.x > widgetOutline.x
			    && mousePos.x < (widgetOutline.x + widgetOutline.width)) {
				if (mousePos.y > widgetOutline.y
				    && mousePos.y < (widgetOutline.y + widgetOutline.height)) {
					setState(isPressed);
				}
			} else { setState(false); }
		}
	}

	void actionsToExecWhenClicked(ActionWhenClicked execFunc) { execFuncPtr = execFunc; }

	void action() override { if (execFuncPtr != nullptr)execFuncPtr(); }

	Button &setScale(float factorX, float factorY) {
		message.setScale(factorX, factorY);
		return *this;
	}

	Button &setScale(sf::Vector2f &factors) {
		message.setScale(factors);
		return *this;
	}

	Button &setText(const std::string &words) {
		message.setMessage(words);
		return *this;
	}

	void render() override {
		GameInfo.getRender()->render(*widgetCurrentPtr);
		GameInfo.getRender()->render(message);
	}
};

#endif //RUNCRAFT_BUTTON_HPP
