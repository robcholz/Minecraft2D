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
	RichText message;
protected:
	void onRender() override {
		Widget::onRender();
		GameInfo.getRender()->render(message);
	}

public:
	explicit ButtonWidget(const std::string& id, int width = 400, int height = 80, bool visible = true, int x = 0, int y = 0)
			: Widget(id, visible) {
		setOutline(&widgetOutline, x, y, width, height);
		loadWidgetTexture(sf::IntRect{0, 66, 200, 20},
		                  sf::IntRect{0, 86, 200, 20});
		widgetSprite.setTexture(widgetNormalTexture);
		widgetSprite.setScale((float) width / 200, (float) height / 20);
		widgetSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);

		message.setFont(gui_style::MessageFont)
		       .setColor(gui_style::MessageColor)
		       .setMessage(TranslatableText::getTranslatable(id, translatable::GUI_BUTTON))
		       .setPosition((float) widgetOutline.x + (float) widgetOutline.width / 2 - message.getGlobalBounds().width,
		                    (float) widgetOutline.y - (float) widgetOutline.height / 8 - 1.0f);
		message.setCharacterSize((int) ((float) widgetOutline.height / 80.0f * 64.0f));
	}

	ButtonWidget& setScale(float factorX, float factorY) {
		message.setScale(factorX, factorY);
		return *this;
	}

	ButtonWidget& setScale(sf::Vector2f& factors) {
		message.setScale(factors);
		return *this;
	}

	ButtonWidget& setText(const std::string& words) {
		message.setMessage(words);
		return *this;
	}
};

#endif //RUNCRAFT_BUTTONWIDGET_HPP
