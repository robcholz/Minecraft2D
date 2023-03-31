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
#include "GUI/text/RichText.hpp"
#include "GUI/screen/Screen.hpp"

class Button : public Widget {
private:
	RichText message;

	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 66, 200, 20));
	inline static std::shared_ptr<sf::IntRect> *intRectClicked = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20));
public:
	explicit Button(const std::string &words, int width = 200, int height = 20, bool visible = true,
	                const sf::Vector2i *position = new sf::Vector2i(0, 0)) {
		this->visible = visible;
		widgetOutlinePosition = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(position->x, position->y));
		widgetSize = new std::shared_ptr<sf::Vector2f>(new sf::Vector2f((float) width, (float) height));

		sliderBackgroundNormal.loadFromFile(widgetAssetPath, **intRectNormal);
		widgetActivated.loadFromFile(widgetAssetPath, **intRectClicked);

		if (visible) {
			widgetCurrentPtr->setTexture(sliderBackgroundNormal, widgetSize);
			widgetCurrentPtr->setScale((float) width / 200, (float) height / 20);
			widgetCurrentPtr->setPosition((float) widgetOutlinePosition->get()->x,
			                              (float) widgetOutlinePosition->get()->y);

			message.setFont(font).setColor(sf::Color::White).setMessage(words);
			message.setPosition(
					(float) widgetOutlinePosition->get()->x + widgetSize->get()->x / 2 -
					message.getGlobalBounds().width,
					(float) position->y - widgetSize->get()->y / 8 - 1.0f);
			message.setCharacterSize((int) ((float) height / 80.0f * 64.0f));
		}
	}

	~Button() {
		delete widgetCurrentPtr;
	}

	void listen(sf::Vector2i mousePos, bool isPressed) override {
		if (visible) {
			if ((float) mousePos.x > widgetCurrentPtr->getPosition().x
			    && (float) mousePos.x < (widgetCurrentPtr->getPosition().x + (float) widgetSize->get()->x)) {
				if ((float) mousePos.y > widgetCurrentPtr->getPosition().y
				    && (float) mousePos.y < (widgetCurrentPtr->getPosition().y + (float) widgetSize->get()->y)) {
					setState(isPressed);
				}
			} else { setState(false); }
		}
	}

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
