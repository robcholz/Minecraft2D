//
// Created by robcholz on 3/8/23.
//

#ifndef MINECRAFT_BUTTONWIDGET_HPP
#define MINECRAFT_BUTTONWIDGET_HPP

#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <utility>
#include "Widget.hpp"
#include "client/gui/style/GUIStyle.hpp"
#include "client/gui/text/RichText.hpp"
#include "resource/TranslatableText.hpp"


class ButtonWidget : public Widget {
private:
	using String = std::string;
protected:
	void onUpdate() override {
		Widget::onUpdate();
		if (identifier->getDotPath() == "gui.button.singleplayer") {
			//TODO: fix this
			//PLOG_DEBUG<<widgetSprite.getGlobalBounds().height;
		}
	}

	void onRender() override {
		Widget::onRender();
		RenderSystem::render(message);
	}

public:
	explicit ButtonWidget(const String& id, int width = 400, int height = 80, bool visible = true, int x = 0, int y = 0)
			: Widget("button." + id, visible) {
		widgetSprite.setScale((float) width / 200, (float) height / 20);
		setOutline(&widgetOutline, widgetSprite, x, y, width, height);
		widgetSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);

		message.setFont(gui_style::MessageFont)
		       .setColor(gui_style::MessageColor)
		       .setMessage(TranslatableText::getTranslatable(*identifier));
		message.setCharacterSize((int) ((float) widgetOutline.height / 2));
		message.setPosition((float) widgetOutline.x + (float) widgetOutline.width / 2 - (float) message.getGlobalBounds().width / 2,
		                    (float) widgetOutline.y + (float) widgetOutline.height / 2 - (float) message.getGlobalBounds().height / 2);
	}

	~ButtonWidget() override = default;

private:
	RichText message;
};

#endif //MINECRAFT_BUTTONWIDGET_HPP
