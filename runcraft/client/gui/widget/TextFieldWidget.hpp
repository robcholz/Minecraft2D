//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TEXTFIELDWIDGET_HPP
#define RUNCRAFT_TEXTFIELDWIDGET_HPP

#include "Widget.hpp"
#include "client/GameInfo.hpp"

class TextFieldWidget : public Widget {
private:
	RichText *text = new RichText(gui_style::MessageFont);

	[[maybe_unused]] static bool activated() { return false; }

	[[maybe_unused]] static bool stateChanged() { return false; }

	[[maybe_unused]] static bool isPressed() { return false; }

	[[maybe_unused]] static bool isClicked() { return false; }

	void updateState(bool state) override {}

	void action() override {}

	void listen(sf::Vector2i mousePosition, bool isPressed) override {}

public:
	explicit TextFieldWidget(const std::string &id, int size, bool visible, int x, int y): Widget(id) {
		this->visible = visible;
		text->setMessage(TranslatableText::getTranslatable(id,translatable::GUI_TEXTFIELD))
				.setCharacterSize(size)
				.setColor(gui_style::MessageColor)
				.setPosition((float) x, (float) y);
		widgetOutline.x = x;
		widgetOutline.y = y;
		widgetOutline.width = (int) text->getGlobalBounds().width;
		widgetOutline.height = (int) text->getGlobalBounds().height;
	}

	~TextFieldWidget() {
		delete text;
	}

	void render() override {
		if (visible) {
			GameInfo.getRender()->render(*text);
		}
	}
};

#endif //RUNCRAFT_TEXTFIELDWIDGET_HPP