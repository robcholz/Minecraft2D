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
#include "../../Game.hpp"
#include "../../GameInfo.hpp"
#include "Widget.hpp"
#include "../text/RichText.hpp"
#include "../screen/Screen.hpp"

class Button : public Widget {
private:
	sf::Texture buttonNormal;
	sf::Texture buttonClicked;
	sf::Sprite *buttonCurrentPtr = new sf::Sprite;
	sf::Font font;
	RichText message;

	std::shared_ptr<sf::IntRect> *intRectNormal;
	std::shared_ptr<sf::IntRect> *intRectClicked;

	std::shared_ptr<sf::Vector2f> *buttonSize;
	std::shared_ptr<sf::Vector2i> *buttonPosition;
	bool lastState = false, stateChange = false;
	bool visible;
public:
	explicit Button(const std::string &words, int width = 200, int height = 20, bool visible = true,
	                const sf::Vector2i *position = new sf::Vector2i(0, 0)) {
		this->visible = visible;
		buttonPosition = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(position->x, position->y));
		buttonSize = new std::shared_ptr<sf::Vector2f>(new sf::Vector2f((float) width, (float) height));

		intRectNormal = new std::shared_ptr<sf::IntRect>(new sf::IntRect(0, 66, 200, 20));
		intRectClicked = new std::shared_ptr<sf::IntRect>(new sf::IntRect(0, 86, 200, 20));

		buttonNormal.loadFromFile(widgetAssetPath, **intRectNormal);
		buttonClicked.loadFromFile(widgetAssetPath, **intRectClicked);
		font.loadFromFile("../resources/font/runcraft.ttf");

		if (visible) {
			buttonCurrentPtr->setTexture(buttonNormal, buttonSize);
			buttonCurrentPtr->setScale((float) width / 200, (float) height / 20);
			buttonCurrentPtr->setPosition((float) buttonPosition->get()->x, (float) buttonPosition->get()->y);

			message.setFont(font).setColor(sf::Color::White).setMessage(words);
			message.setPosition(
					(float) buttonPosition->get()->x + buttonSize->get()->x / 2 - message.getGlobalBounds().width,
					(float) position->y - buttonSize->get()->y / 8 - 1.0f);
			message.setCharacterSize((int) ((float) height / 80.0f * 64.0f));
		}

		renderAble.drawable = buttonCurrentPtr;
		renderAble.text = &message;
	}

	~Button() {
		delete buttonCurrentPtr;
	}

	void listen(sf::Vector2i mousePos, bool isPressed) override {
		if (visible) {
			if ((float) mousePos.x > buttonCurrentPtr->getPosition().x
			    && (float) mousePos.x < (buttonCurrentPtr->getPosition().x + (float) buttonSize->get()->x)) {
				if ((float) mousePos.y > buttonCurrentPtr->getPosition().y
				    && (float) mousePos.y < (buttonCurrentPtr->getPosition().y + (float) buttonSize->get()->y)) {
					setState(isPressed);
				}
			} else { setState(false); }
		}
	}

	void setState(bool state) {
		if (lastState != state) {
			stateChange = true;
			lastState = state;
			if (lastState) {
				buttonCurrentPtr->setTexture(buttonClicked);
				return;
			} else { buttonCurrentPtr->setTexture(buttonNormal); }
		} else stateChange = false;
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

	Button &setVisibility(bool visibility) {
		visible = visibility;
		return *this;
	}


	/*
	void optionsOnClicked(Screen *screen) {

	}
	 */

	[[maybe_unused]] Vector2D<int> *getSize() const { return reinterpret_cast<Vector2D<int> *>(buttonSize->get()); }

	bool pressed() override { return lastState; }

	bool stateChanged() override { return stateChange; }

	void render() override {
		GameInfo.getRender()->render(*renderAble.drawable);
		GameInfo.getRender()->render(*renderAble.text);
	}

	RenderAble *getWidgetRenderAble() override {
		return &renderAble;
	}
};

#endif //RUNCRAFT_BUTTON_HPP
