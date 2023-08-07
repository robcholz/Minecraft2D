//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_TEXTFIELDWIDGET_HPP
#define MINECRAFT_TEXTFIELDWIDGET_HPP

#include "Widget.hpp"


class TextFieldWidget : public Widget {
protected:
	void onRender() override {
		RenderSystem::render(text);
	}

public:
	explicit TextFieldWidget(const String& id, int size, bool visible, int x, int y) :
			Widget("textfield."+id,visible) {
		text.setColor(gui_style::MessageColor)
		    .setMessage(TranslatableText::getTranslatable(*identifier))
		    .setCharacterSize(size)
		    .setPosition((float) x, (float) y);
		setOutline(&widgetOutline, widgetSprite, x, y, (int) text.getGlobalBounds().width, (int) text.getGlobalBounds().height);
	}

	~TextFieldWidget() override = default;

	bool stateChanged() = delete;

	bool activated() = delete;

	bool isPressed() = delete;

	bool isClicked() = delete;

private:
	RichText text{gui_style::MessageFont};

	void executeCallbackFunc() override {}

	void onUpdate() override {}
};

#endif //MINECRAFT_TEXTFIELDWIDGET_HPP
