//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TEXT_HPP
#define RUNCRAFT_TEXT_HPP

#include "Widget.hpp"
#include "GUI/text/RichText.hpp"
#include "GUI/GUI.hpp"
#include "GameInfo.hpp"

class Text : public Widget {
private:
	RichText *text = new RichText(gui_style::MessageFont);

	[[maybe_unused]] bool activated() { return false; }

	[[maybe_unused]] bool stateChanged() { return false; }

	[[maybe_unused]] bool isPressed() { return false; }

	[[maybe_unused]] bool isClicked() { return false; }

	void setState(bool state) override {}

	void action() override {}

	void listen(sf::Vector2i mousePosition, bool isPressed) override {}

public:
	explicit Text(const std::string &message, int size, bool visible, int x, int y) {
		this->visible = visible;
		text->setMessage(message)
				.setCharacterSize(size)
				.setColor(gui_style::MessageColor)
				.setPosition((float) x, (float) y);
		widgetOutline.x = x;
		widgetOutline.y = y;
		widgetOutline.width = (int) text->getGlobalBounds().width;
		widgetOutline.height = (int) text->getGlobalBounds().height;
	}

	~Text() {
		delete text;
	}

	void render() override {
		if (visible) {
			GameInfo.getRender()->render(*text);
		}
	}
};

#endif //RUNCRAFT_TEXT_HPP
