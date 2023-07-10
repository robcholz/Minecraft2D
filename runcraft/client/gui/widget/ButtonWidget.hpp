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
	using String = std::string;
protected:
	void onRender() override {
		Widget::onRender();
		GameInfo.getRender()->render(message);
	}

public:
	explicit ButtonWidget(const String& id, int width = 400, int height = 80, bool visible = true, int x = 0, int y = 0)
			: Widget("button." + id, visible) {
		widgetSprite.setScale((float) width / 200, (float) height / 20);
		setOutline(&widgetOutline, widgetSprite, x, y, width, height);
		widgetSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);

		message.setFont(gui_style::MessageFont)
		       .setColor(gui_style::MessageColor)
		       .setMessage(TranslatableText::getTranslatable(*identifier))
		       .setPosition((float) widgetOutline.x + (float) widgetOutline.width / 2 - message.getGlobalBounds().width,
		                    (float) widgetOutline.y - (float) widgetOutline.height / 8 - 1.0f);
		message.setCharacterSize((int) ((float) widgetOutline.height / 80.0f * 64.0f));
	}

	~ButtonWidget() override = default;

private:
	RichText message;
};

#endif //RUNCRAFT_BUTTONWIDGET_HPP
