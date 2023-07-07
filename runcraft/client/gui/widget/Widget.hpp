//
// Created by robcholz on 3/24/23.
//

#ifndef RUNCRAFT_WIDGET_HPP
#define RUNCRAFT_WIDGET_HPP

#pragma once

class Screen;

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <vector>
#include "util/math/MathHelper.hpp"
#include "client/input/Input.hpp"

class Widget : public GUI {
private:
	bool mouseButtonPressed{};
	sf::Vector2i mousePosition{};

protected:
	using CallbackFunc = std::function<void()>;
	using String = std::string;
	sf::Texture widgetNormalTexture, widgetFocusedTexture;
	sf::IntRect widgetNormalIntRect, widgetFocusedIntRect;
	sf::Sprite widgetSprite;
	Areai widgetOutline{};
	static inline String widgetPath = Identifier::guiPath + "widgets.png";
	std::unique_ptr<Identifier> identifier;
	CallbackFunc execFuncPtr = nullptr;
	bool visible = true;
	bool lastClicked = false, clicked = false, pressed = false;
	bool focus = false;
	bool active = true;
	bool stateChanged = false;
	input::MouseObserver mouseObserver{};

	static void setOutline(Areai* outline, const sf::Sprite& sprite, int x, int y, int width, int height) {
		outline->x = x;
		outline->y = y;
		outline->width = (int) sprite.getGlobalBounds().width;
		outline->height = (int) sprite.getGlobalBounds().height;
	}

	static void setSpriteScale(sf::Sprite& sprite) {
		auto mapVal = GameInfo.getConstExternalData()->windowState.actualPixelToOnePixel;
		sprite.setScale(mapVal, mapVal);
	}

	void loadWidgetTexture(const sf::IntRect& widgetNormal, const sf::IntRect& widgetActivated, const String& path) {
		widgetNormalIntRect = widgetNormal;
		widgetFocusedIntRect = widgetActivated;
		widgetNormalTexture.loadFromFile(path, widgetNormalIntRect);
		widgetFocusedTexture.loadFromFile(path, widgetFocusedIntRect);
	}

	void loadWidgetTexture(const String& path) {
		widgetNormalTexture.loadFromFile(path);
		widgetFocusedTexture.loadFromFile(path);
	}

	virtual void setPressed(bool isPressed) {
		pressed = isPressed;
	}

	virtual void setClicked(bool isClicked) {
		setStateChanged(lastClicked != isClicked);
		clicked = (isStateChanged() && isClicked);
		lastClicked = isClicked;
	}

	bool isMouseButtonPressed() const { return mouseButtonPressed; }

	sf::Vector2i getMousePosition() const { return mousePosition; }

	void setVisible(bool visibility) { visible = visibility; }

	void setActive(bool activity) { active = activity; }

	void setFocused(bool isFocused = true) {
		focus = isFocused;
		if (isFocused) widgetSprite.setTexture(widgetFocusedTexture);
		else widgetSprite.setTexture(widgetNormalTexture);
	}

	void setStateChanged(bool isStateChanged) { stateChanged = isStateChanged; }

	void updateMouse() {
		mouseButtonPressed = mouseObserver.isActivated();
		mousePosition = mouseObserver.getPosition();
	}

	virtual void onRender() {
		GameInfo.getRender()->render(widgetSprite);
	}

	virtual void onUpdate() {
		auto inInWidgetBoundary = checkVectorBoundary(getMousePosition(), widgetOutline);
		setPressed(inInWidgetBoundary && isMouseButtonPressed());
		setClicked(isMouseButtonPressed());
		setFocused(inInWidgetBoundary);
	}

public:
	explicit Widget(const String& id, bool visible, const String& widgetAssetPath = widgetPath,
	                const sf::IntRect& widgetNormal = {0, 66, 200, 20},
	                const sf::IntRect& widgetActivated = {0, 86, 200, 20}) {
		mouseObserver.attachKey(input::MouseKeyType::Left);
		this->identifier = std::make_unique<Identifier>(id, Identifier::Category::GUI);
		this->visible = visible;
		loadWidgetTexture(widgetNormal, widgetActivated, widgetAssetPath);
		widgetSprite.setTexture(widgetNormalTexture);
	}

	explicit Widget(const String& id) {
		mouseObserver.attachKey(input::MouseKeyType::Left);
		this->identifier = std::make_unique<Identifier>(id, Identifier::Category::GUI);
		this->visible = true;
	}


	bool isVisible() const { return visible; }

	bool isActive() const { return active; }

	bool isClicked() const { return clicked; }

	bool isPressed() const { return pressed; }

	bool isFocused() const { return focus; }

	bool isStateChanged() const { return stateChanged; }

	virtual void executeCallbackFunc() { if (execFuncPtr != nullptr) execFuncPtr(); }

	Widget& executeFuncWhenActivated(const CallbackFunc& execFunc) {
		execFuncPtr = execFunc;
		return *this;
	}

	void activate() {
		setActive(true);
	}

	void deactivate() {
		setActive(false);
	}

	void show() {
		setActive(true);
		setVisible(true);
	}

	void hide() {
		setActive(false);
		setVisible(false);
	}

	void update() {
		if (isActive()) {
			updateMouse();
			onUpdate();
		}
	}

	void render() override {
		if (isVisible())
			onRender();
	}
};

#endif //RUNCRAFT_WIDGET_HPP
