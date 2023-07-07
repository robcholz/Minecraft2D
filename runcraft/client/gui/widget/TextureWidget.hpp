//
// Created by robcholz on 7/6/23.
//

#ifndef RUNCRAFT_TEXTUREWIDGET_HPP
#define RUNCRAFT_TEXTUREWIDGET_HPP

#include "Widget.hpp"
#include "util/Identifier.hpp"

class TextureWidget: public Widget{
protected:
	void onUpdate() override{}
public:
	explicit TextureWidget(const String& id, bool visible = true, int x = 0, int y = 0)
			: Widget(id) {
		this->visible=visible;
		loadWidgetTexture(identifier->getAbsolutePath());
		widgetSprite.setTexture(widgetNormalTexture);
		setOutline(&widgetOutline, widgetSprite,x, y, widgetSprite.getGlobalBounds().width, widgetSprite.getGlobalBounds().height);
		widgetSprite.setScale(2.1,2.1);
		widgetSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);
	}
};

#endif //RUNCRAFT_TEXTUREWIDGET_HPP
