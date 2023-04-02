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
#include "GUI/text/RichText.hpp"
#include "GUI/Style/GUIStyle.hpp"

class Slider : public Widget {
protected:
	typedef unsigned short ButtonValue;
	Areai sliderBackgroundOutline{};
	Intervali sliderOutlineBound{};
	ButtonValue sliderValue = 100;
	std::string title;
	RichText message;
private:
	sf::Sprite sliderBackgroundSprite;
	sf::Texture sliderBackgroundTexture;
	bool isInBackgroundBoundary = false, isInSliderBoundary = false;
	bool sliderLock = true;

	inline static std::shared_ptr<sf::IntRect> *intRectBackground = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 46, 200, 20)); // background blur
	inline static std::shared_ptr<sf::IntRect> *intRectSliderNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 66, 200, 20)); // slider blur
	inline static std::shared_ptr<sf::IntRect> *intRectSliderActivated = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20)); // slider component
public:
	explicit Slider(const std::string &words, int width = 200, int height = 20, bool visible = true, int x = 0, int y = 0) : Widget() {
		title = words;
		this->visible = visible;
		widgetSize = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(width, height));

		/*background*/
		sliderBackgroundTexture.loadFromFile(widgetAssetPath, **intRectBackground);
		/* sliders*/
		widgetNormalTexture.loadFromFile(widgetAssetPath, **intRectSliderNormal);
		widgetActivatedTexture.loadFromFile(widgetAssetPath, **intRectSliderActivated);

		sliderBackgroundSprite.setTexture(sliderBackgroundTexture, widgetSize); // background
		sliderBackgroundSprite.setScale((float) width / 200, (float) height / 20);
		sliderBackgroundSprite.setPosition((float) x, (float) y);

		widgetCurrentSprite.setTexture(widgetNormalTexture, widgetSize);
		widgetCurrentSprite.scale((float) 0.15f, (float) height / 20);
		widgetCurrentSprite.setPosition((float) x * 1.5f, (float) y);

		sliderBackgroundOutline.x = x;
		sliderBackgroundOutline.y = y;
		sliderBackgroundOutline.width = width;
		sliderBackgroundOutline.height = height;
		widgetOutline.x = (int) widgetCurrentSprite.getGlobalBounds().left;
		widgetOutline.y = (int) widgetCurrentSprite.getGlobalBounds().top;
		widgetOutline.width = (int) widgetCurrentSprite.getGlobalBounds().width;
		widgetOutline.height = (int) widgetCurrentSprite.getGlobalBounds().height;

		sliderOutlineBound.lower = (int) sliderBackgroundSprite.getPosition().x + 4;
		sliderOutlineBound.upper = (int) sliderBackgroundSprite.getPosition().x
		                           + (int) sliderBackgroundSprite.getGlobalBounds().width
		                           - (int) widgetCurrentSprite.getGlobalBounds().width - 4;
		message.setFont(gui_style::MessageFont)
				.setColor(gui_style::MessageColor)
				.setMessage(title + ": " + std::to_string(getValue()) + "%");
		updateMessagePosition();
		message.setCharacterSize((int) ((float) height / 80.0f * 64.0f));
	}

	void updateMessagePosition() {
		message.setPosition((float) sliderBackgroundOutline.x + (float) sliderBackgroundOutline.width / 2 - message.getGlobalBounds().width,
		                    (float) widgetOutline.y - (float) widgetOutline.height / 8 - 1.0f);
	}

	void action() override {}

	void cursorBackgroundIntersectionCheck(sf::Vector2i mousePos) {
		isInBackgroundBoundary = checkVectorBoundary(mousePos.x, mousePos.y, sliderBackgroundOutline.x, sliderBackgroundOutline.y,
		                                             sliderBackgroundOutline.width,
		                                             sliderBackgroundOutline.height);
	}

	bool sliderBoundaryCheck(int x) const {
		return ((sliderOutlineBound.lower < x) && (x < sliderOutlineBound.upper));
	}

	void cursorSliderIntersectionCheck(sf::Vector2i mousePos) {
		isInSliderBoundary = checkVectorBoundary(mousePos.x, mousePos.y, widgetOutline.x, widgetOutline.y, widgetOutline.width,
		                                         widgetOutline.height);
	}

	void updatePosition(int x) {
		if (sliderBoundaryCheck(x)) {
			widgetOutline.x = x;
			widgetCurrentSprite.setPosition((float) x, (float) widgetOutline.y);
		}
	}

	void listen(sf::Vector2i mousePos, bool isPressed) override {
		if (getVisibility()) {
			cursorBackgroundIntersectionCheck(mousePos);
			cursorSliderIntersectionCheck(mousePos);
			if (isInSliderBoundary && isPressed || isInBackgroundBoundary && isPressed) { sliderLock = false; }
			if (!isPressed) { sliderLock = true; }
			updateState(!sliderLock);
			if (!sliderLock) {
				action();
				widgetCurrentSprite.setTexture(widgetActivatedTexture);
				updatePosition(mousePos.x);
			} else widgetCurrentSprite.setTexture(widgetNormalTexture);
		}
	}

	ButtonValue getValue() const { return sliderValue; }

	void render() override {
		GameInfo.getRender()->render(sliderBackgroundSprite);
		GameInfo.getRender()->render(widgetCurrentSprite);
		GameInfo.getRender()->render(message);
	}
};

#endif //RUNCRAFT_SLIDER_HPP
