//
// Created by robcholz on 3/26/23.
//

#ifndef RUNCRAFT_SLIDER_HPP
#define RUNCRAFT_SLIDER_HPP

#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include "Widget.hpp"
#include "GameInfo.hpp"
#include "util/Math_Helper.hpp"
#include "GUI/text/Text.hpp"

class Slider : public Widget {
protected:
	typedef unsigned short ButtonValue;
	Areai sliderOutline{};
	Intervali sliderOutlineBound{};
	ButtonValue sliderValue = 100;
	std::string title;
	Text message;
private:
	sf::Sprite *sliderBackgroundPtr = new sf::Sprite;
	sf::Texture widgetNormal;
	bool isInBackgroundBoundary = false, isInSliderBoundary = false;
	bool sliderLock = true;


	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 46, 200, 20)); // background blur
	inline static std::shared_ptr<sf::IntRect> *intRectSliderNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 66, 200, 20)); // background blur
	inline static std::shared_ptr<sf::IntRect> *intRectSliderActivated = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20)); // slider component
	inline static std::shared_ptr<sf::Color> *backgroundMessageColor = new std::shared_ptr<sf::Color>(
			new sf::Color(220, 220, 220, 255));
public:
	explicit Slider(const std::string &words, int width = 200, int height = 20, bool visible = true, int x = 0, int y = 0) : Widget() {
		title = words;
		this->visible = visible;
		widgetSize = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(width, height));

		/*background*/
		sliderBackgroundNormal.loadFromFile(widgetAssetPath, **intRectNormal);
		/* sliders*/
		widgetNormal.loadFromFile(widgetAssetPath, **intRectSliderNormal);
		widgetActivated.loadFromFile(widgetAssetPath, **intRectSliderActivated);

		sliderBackgroundPtr->setTexture(sliderBackgroundNormal, widgetSize); // background
		sliderBackgroundPtr->setScale((float) width / 200, (float) height / 20);
		sliderBackgroundPtr->setPosition((float) x, (float) y);

		widgetCurrentPtr->setTexture(widgetNormal, widgetSize);
		widgetCurrentPtr->scale((float) 0.15f, (float) height / 20);
		widgetCurrentPtr->setPosition((float) x * 1.5f, (float) y);

		widgetOutline.x = x;
		widgetOutline.y = y;
		widgetOutline.width = width;
		widgetOutline.height = height;
		sliderOutline.x = (int) widgetCurrentPtr->getGlobalBounds().left;
		sliderOutline.y = (int) widgetCurrentPtr->getGlobalBounds().top;
		sliderOutline.width = (int) widgetCurrentPtr->getGlobalBounds().width;
		sliderOutline.height = (int) widgetCurrentPtr->getGlobalBounds().height;

		sliderOutlineBound.lower = (int) sliderBackgroundPtr->getPosition().x + 4;
		sliderOutlineBound.upper = (int) sliderBackgroundPtr->getPosition().x
		                           + (int) sliderBackgroundPtr->getGlobalBounds().width
		                           - (int) widgetCurrentPtr->getGlobalBounds().width - 4;
		message.setFont(font)
				.setColor(**backgroundMessageColor)
				.setMessage(title + ": " + std::to_string(getValue()) + "%");
		updateMessagePosition();
		message.setCharacterSize((int) ((float) height / 80.0f * 64.0f));
	}

	~Slider() {
		delete widgetCurrentPtr;
		delete sliderBackgroundPtr;
	}

	void setState(bool state) override {
		if (lastState != state) {
			stateChange = true;
			lastState = state;
			if (activated()) {
				widgetCurrentPtr->setTexture(widgetActivated);
				return;
			} else { widgetCurrentPtr->setTexture(widgetNormal); }
		} else {
			stateChange = false;
		}
	}

	void updateMessagePosition() {
		message.setPosition((float) widgetOutline.x + (float) widgetOutline.width / 2 - message.getGlobalBounds().width,
		                    (float) sliderOutline.y - (float) sliderOutline.height / 8 - 1.0f);
	}

	void action() override {}

	void cursorBackgroundIntersectionCheck(sf::Vector2i mousePos) {
		isInBackgroundBoundary = checkVectorBoundary(mousePos.x, mousePos.y, widgetOutline.x, widgetOutline.y, widgetOutline.width,
		                                             widgetOutline.height);
	}

	bool sliderBoundaryCheck(int x) const {
		return ((sliderOutlineBound.lower < x) && (x < sliderOutlineBound.upper));
	}

	void cursorSliderIntersectionCheck(sf::Vector2i mousePos) {
		isInSliderBoundary = checkVectorBoundary(mousePos.x, mousePos.y, sliderOutline.x, sliderOutline.y, sliderOutline.width,
		                                         sliderOutline.height);
	}

	void updatePosition(int x) {
		if (sliderBoundaryCheck(x)) {
			sliderOutline.x = x;
			widgetCurrentPtr->setPosition((float) x, (float) sliderOutline.y);
		}
	}

	void listen(sf::Vector2i mousePos, bool isPressed) override {
		if (visible) {
			cursorBackgroundIntersectionCheck(mousePos);
			cursorSliderIntersectionCheck(mousePos);
			if (isInSliderBoundary && isPressed || isInBackgroundBoundary && isPressed) { sliderLock = false; }
			if (!isPressed) { sliderLock = true; }
			if (!sliderLock) {
				action();
				updatePosition(mousePos.x);
			}
			setState(!sliderLock);
		}
	}

	ButtonValue getValue() const { return sliderValue; }

	void render() override {
		GameInfo.getRender()->render(*sliderBackgroundPtr);
		GameInfo.getRender()->render(*widgetCurrentPtr);
		GameInfo.getRender()->render(message);
	}
};

#endif //RUNCRAFT_SLIDER_HPP
