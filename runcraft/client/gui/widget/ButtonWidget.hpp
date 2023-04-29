//
// Created by robcholz on 3/8/23.
//

#ifndef RUNCRAFT_BUTTONWIDGET_HPP
#define RUNCRAFT_BUTTONWIDGET_HPP

#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <utility>
#include "client/GameInfo.hpp"
#include "Widget.hpp"
#include "client/gui/style/GUIStyle.hpp"
#include "client/gui/text/RichText.hpp"
#include "resource/TranslatableText.hpp"

class ButtonWidget : public Widget {
private:
	ActionWhenActivated execFuncPtr = nullptr;

	RichText message;

	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 66, 200, 20));
	inline static std::shared_ptr<sf::IntRect> *intRectClicked = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 86, 200, 20));
public:
	explicit ButtonWidget(const std::string &id, int width = 400, int height = 80, bool visible = true, int x = 0, int y = 0) : Widget(id) {
		this->visible = visible;
		widgetOutline.x = x;
		widgetOutline.y = y;
		widgetOutline.width = width;
		widgetOutline.height = height;
		widgetSize = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(width, height));

		widgetNormalTexture.loadFromFile(widgetAssetPath, **intRectNormal);
		widgetActivatedTexture.loadFromFile(widgetAssetPath, **intRectClicked);

		widgetCurrentSprite.setTexture(widgetNormalTexture, widgetSize);
		widgetCurrentSprite.setScale((float) width / 200, (float) height / 20);
		widgetCurrentSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);

		message.setFont(gui_style::MessageFont)
		       .setColor(gui_style::MessageColor)
		       .setMessage(TranslatableText::getTranslatable(id, translatable::GUI_BUTTON))
		       .setPosition((float) widgetOutline.x + (float) widgetOutline.width / 2 - message.getGlobalBounds().width,
		                    (float) widgetOutline.y - (float) widgetOutline.height / 8 - 1.0f);
		message.setCharacterSize((int) ((float) widgetOutline.height / 80.0f * 64.0f));
	}

	void actionsToExecWhenClicked(const ActionWhenActivated &execFunc) { execFuncPtr = execFunc; }

	void action() override { if (execFuncPtr != nullptr)execFuncPtr(); }

	ButtonWidget &setScale(float factorX, float factorY) {
		message.setScale(factorX, factorY);
		return *this;
	}

	ButtonWidget &setScale(sf::Vector2f &factors) {
		message.setScale(factors);
		return *this;
	}

	ButtonWidget &setText(const std::string &words) {
		message.setMessage(words);
		return *this;
	}

	void render() override {
		if (getVisibility()) {
			GameInfo.getRender()->render(widgetCurrentSprite);
			GameInfo.getRender()->render(message);
		}
	}
};

#endif //RUNCRAFT_BUTTONWIDGET_HPP
