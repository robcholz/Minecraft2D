//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_TEXTUREDBUTTONWIDGET_HPP
#define RUNCRAFT_TEXTUREDBUTTONWIDGET_HPP

#include "Widget.hpp"
#include "client/GameInfo.hpp"

class TexturedButtonWidget : public Widget {
private:
	ActionWhenActivated execFuncPtr = nullptr;
	RichText message;
	inline static std::shared_ptr<sf::IntRect> *intRectNormal = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 106, 20, 20));
	inline static std::shared_ptr<sf::IntRect> *intRectClicked = new std::shared_ptr<sf::IntRect>(
			new sf::IntRect(0, 126, 20, 20));
public:
	explicit TexturedButtonWidget(int width=32 , int height=32, bool visible=true, int x = 0, int y = 0) {
		this->visible = visible;
		widgetSize = new std::shared_ptr<sf::Vector2i>(new sf::Vector2i(width, height));

		widgetNormalTexture.loadFromFile(widgetAssetPath, **intRectNormal);
		widgetActivatedTexture.loadFromFile(widgetAssetPath, **intRectClicked);
		widgetCurrentSprite.setTexture(widgetNormalTexture, widgetSize);
		widgetCurrentSprite.setScale((float) width / 8, (float) height / 8);
		widgetOutline.x = x;
		widgetOutline.y = y;
		widgetOutline.width = (int)widgetCurrentSprite.getGlobalBounds().width;
		widgetOutline.height = (int)widgetCurrentSprite.getGlobalBounds().height;
		widgetCurrentSprite.setPosition((float) widgetOutline.x, (float) widgetOutline.y);
	}

	void actionsToExecWhenClicked(ActionWhenActivated execFunc) { execFuncPtr = std::move(execFunc); }

	void action() override { if (execFuncPtr != nullptr)execFuncPtr(); }

	TexturedButtonWidget &setScale(float factorX, float factorY) {

		return *this;
	}

	TexturedButtonWidget &setScale(sf::Vector2f &factors) {

		return *this;
	}

	void render() override {
		if (getVisibility()) {
			GameInfo.getRender()->render(widgetCurrentSprite);
		}
	}
};

#endif //RUNCRAFT_TEXTUREDBUTTONWIDGET_HPP
