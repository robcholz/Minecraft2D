//
// Created by robcholz on 3/26/23.
//

#ifndef RUNCRAFT_SLIDER_HPP
#define RUNCRAFT_SLIDER_HPP

#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include "Widget.hpp"
#include "../../GameInfo.hpp"

class Slider : public Widget {
private:
	RichText message;

	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 46, 200, 20));
	inline static std::shared_ptr<sf::IntRect> *intRectActivated = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20));

	// std::shared_ptr<sf::Vector2i> *sliderPosition;
public:
	explicit Slider(const std::string &words, int width = 200, int height = 20, bool visible = true,
	                const sf::Vector2i *position = new sf::Vector2i(0, 0)) : Widget() {
		this->visible = visible;
		widgetOutlinePosition = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(position->x, position->y));
		widgetSize = new std::shared_ptr<sf::Vector2f>(new sf::Vector2f((float) width, (float) height));

		widgetNormal.loadFromFile(widgetAssetPath, **intRectNormal);
		widgetActivated.loadFromFile(widgetAssetPath, **intRectActivated);

		if (visible) {
			widgetCurrentPtr->setTexture(widgetNormal, widgetSize);
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

	~Slider() {
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

	/*
	void optionsOnClicked(Screen *screen) {

	}
	 */

	void render() override {
		GameInfo.getRender()->render(*widgetCurrentPtr);
		GameInfo.getRender()->render(message);
	}

};

#endif //RUNCRAFT_SLIDER_HPP
