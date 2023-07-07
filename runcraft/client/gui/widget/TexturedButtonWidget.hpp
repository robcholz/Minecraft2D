//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TEXTUREDBUTTONWIDGET_HPP
#define RUNCRAFT_TEXTUREDBUTTONWIDGET_HPP

#include "Widget.hpp"
#include "client/GameInfo.hpp"

class TexturedButtonWidget : public Widget {
private:
	CallbackFunc execFuncPtr = nullptr;
	RichText message;
public:
	explicit TexturedButtonWidget(int width = 32, int height = 32, bool visible = true, int x = 0, int y = 0)
			: Widget("", visible, widgetPath,
			         {0, 106, 20, 20},
			         {0, 126, 20, 20}) {
		widgetSprite.setScale((float) width / 8, (float) height / 8);
		setOutline(&widgetOutline, widgetSprite, x, y, (int) widgetSprite.getGlobalBounds().width, (int) widgetSprite.getGlobalBounds().height);
		widgetSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);
	}
};

#endif //RUNCRAFT_TEXTUREDBUTTONWIDGET_HPP
