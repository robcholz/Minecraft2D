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
	sf::Texture widgetNormal;
	sf::Sprite *sliderBackgroundPtr = new sf::Sprite;
	RichText message;
	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 46, 200, 20)); // background blur
	inline static std::shared_ptr<sf::IntRect> *intRectSliderNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 66, 200, 20)); // background blur
	inline static std::shared_ptr<sf::IntRect> *intRectSliderActivated = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20)); // slider component
	inline static std::shared_ptr<sf::Color> *backgroundMessageColor = new std::shared_ptr<sf::Color>(
			new sf::Color(220, 220, 220, 255));

	int sliderPosX = 0, sliderPosY = 0;
public:
	explicit Slider(const std::string &words, int width = 200, int height = 20, bool visible = true,
	                const sf::Vector2i *position = new sf::Vector2i(0, 0)) : Widget() {
		this->visible = visible;
		widgetOutlinePosition = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(position->x, position->y));
		sliderPosX = (int) ((float) position->x * 1.5f);
		sliderPosY = position->y;
		widgetSize = new std::shared_ptr<sf::Vector2f>(new sf::Vector2f((float) width, (float) height));

		/*background*/
		sliderBackgroundNormal.loadFromFile(widgetAssetPath, **intRectNormal);
		/* sliders*/
		widgetNormal.loadFromFile(widgetAssetPath, **intRectSliderNormal);
		widgetActivated.loadFromFile(widgetAssetPath, **intRectSliderActivated);

		if (visible) {
			sliderBackgroundPtr->setTexture(sliderBackgroundNormal, widgetSize); // background
			sliderBackgroundPtr->setScale((float) width / 200, (float) height / 20);
			sliderBackgroundPtr->setPosition((float) widgetOutlinePosition->get()->x,
			                                 (float) widgetOutlinePosition->get()->y);

			widgetCurrentPtr->setTexture(widgetNormal, widgetSize);
			widgetCurrentPtr->setScale((float) 0.15f, (float) height / 20);
			widgetCurrentPtr->setPosition((float) sliderPosX, (float) sliderPosY);

			message.setFont(font).setColor(**backgroundMessageColor).setMessage(words);
			message.setPosition(
					(float) widgetOutlinePosition->get()->x + widgetSize->get()->x / 2 -
					message.getGlobalBounds().width,
					(float) position->y - widgetSize->get()->y / 8 - 1.0f);
			message.setCharacterSize((int) ((float) height / 80.0f * 64.0f));
		}
	}

	~Slider() {
		delete widgetCurrentPtr;
		delete sliderBackgroundPtr;
	}

	void setState(bool state) override {
		if (lastState != state) {
			stateChange = true;
			lastState = state;
			if (lastState) {
				widgetCurrentPtr->setTexture(widgetActivated);
				return;
			} else { widgetCurrentPtr->setTexture(widgetNormal); }
		} else stateChange = false;
	}

	void sliderUpdatePos(sf::Vector2i mousePos, bool isPressed) {
		if (isPressed) {
			if ((mousePos.x > widgetOutlinePosition->get()->x) &&
			    (mousePos.x < widgetOutlinePosition->get()->x + (int) widgetSize->get()->x -
			                  (int) widgetCurrentPtr->getGlobalBounds().width)) {
				sliderPosX = mousePos.x;
			}
			widgetCurrentPtr->setPosition((float) sliderPosX, (float) sliderPosY);
		}
	}

	void listen(sf::Vector2i mousePos, bool isPressed) override {
		if (visible) {
			if ((float) mousePos.x > widgetCurrentPtr->getPosition().x
			    && (float) mousePos.x < (widgetCurrentPtr->getPosition().x + (float) widgetSize->get()->x)) {
				if ((float) mousePos.y > widgetCurrentPtr->getPosition().y
				    && (float) mousePos.y < (widgetCurrentPtr->getPosition().y + (float) widgetSize->get()->y)) {
					sliderUpdatePos(mousePos, isPressed);
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
		GameInfo.getRender()->render(*sliderBackgroundPtr);
		GameInfo.getRender()->render(*widgetCurrentPtr);
		GameInfo.getRender()->render(message);
	}

};

#endif //RUNCRAFT_SLIDER_HPP
