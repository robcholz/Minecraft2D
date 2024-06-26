//
// Created by robcholz on 3/26/23.
//
#pragma once

#ifndef MINECRAFT_SLIDERWIDGET_HPP
#define MINECRAFT_SLIDERWIDGET_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "Widget.hpp"
#include "util/math/MathHelper.hpp"

class SliderWidget : public Widget {
 protected:
  using ButtonValue = unsigned short;
  Intervali widgetOutlineBound{};
  RichText message;
  std::wstring title;
  Areai widgetBackgroundOutline{};
  ButtonValue sliderValue = 100;

  static sf::String getDisplayContent(const std::wstring& ptitle, int val) {
    return {ptitle + L": " + std::to_wstring(val) + L"%"};
  }

  bool isMoving() const { return moving; }

  virtual void onMove() {
    activate();
    executeCallbackFunc();
    updateWidgetPosition(getMousePosition().x);
  }

  void onUpdate() override {
    isInBackgroundBoundary =
        checkVectorBoundary(getMousePosition(), widgetBackgroundOutline);
    setPressed(isMouseButtonPressed() && isInBackgroundBoundary);
    setClicked(isMouseButtonPressed());
    setFocused(isInBackgroundBoundary);
    if (isPressed() && isStateChanged())
      setMoving(true);
    if (!isPressed())
      setMoving(false);
    if (isMoving())
      onMove();
  }

  void onRender() override {
    RenderSystem::render(sliderBackgroundSprite);
    Widget::onRender();
    RenderSystem::render(message);
  }

 public:
  explicit SliderWidget(const String& id,
                        int width = 200,
                        int height = 20,
                        bool visible = true,
                        int x = 0,
                        int y = 0)
      : Widget("slider." + id, visible), message(gui_style::getMessageFont()) {
    title = TranslatableText::getTranslatable(*identifier);
    /*background*/
    sliderBackgroundTexture.loadFromFile(Path::widgetPath, intRectBackground);
    sliderBackgroundSprite.setTexture(sliderBackgroundTexture);  // background
    sliderBackgroundSprite.setScale((float)width / 200, (float)height / 20);
    sliderBackgroundSprite.setPosition((float)x, (float)y);

    widgetSprite.scale((float)0.15f, (float)height / 20);
    widgetSprite.setPosition((float)x * 1.5f, (float)y);

    setOutline(&widgetBackgroundOutline, sliderBackgroundSprite, x, y, width,
               height);
    setOutline(&widgetOutline, widgetSprite,
               (int)widgetSprite.getGlobalBounds().left,
               (int)widgetSprite.getGlobalBounds().top,
               (int)widgetSprite.getGlobalBounds().width,
               (int)widgetSprite.getGlobalBounds().height);

    widgetOutlineBound.lower = (int)sliderBackgroundSprite.getPosition().x + 4;
    widgetOutlineBound.upper =
        (int)sliderBackgroundSprite.getPosition().x +
        (int)sliderBackgroundSprite.getGlobalBounds().width -
        (int)widgetSprite.getGlobalBounds().width - 4;
    message.setFont(gui_style::getMessageFont())
        .setColor(gui_style::MessageColor)
        .setMessage(getDisplayContent(title, getValue()));
    message.setCharacterSize((int)((float)widgetBackgroundOutline.height / 2));
    updateMessagePosition();
  }

  ~SliderWidget() override = default;

  ButtonValue getValue() const { return sliderValue; }

 private:
  sf::Sprite sliderBackgroundSprite;
  sf::Texture sliderBackgroundTexture;
  bool isInBackgroundBoundary = false;
  bool moving = false;

  const static inline sf::IntRect intRectBackground{0, 46, 200,
                                                    20};  // background blur

  void updateMessagePosition() {
    message.setPosition((float)widgetBackgroundOutline.x +
                            (float)widgetBackgroundOutline.width / 2 -
                            (float)message.getGlobalBounds().width / 2,
                        (float)widgetBackgroundOutline.y +
                            (float)widgetBackgroundOutline.height / 2 -
                            (float)message.getGlobalBounds().height / 2);
  }

  void updateWidgetPosition(int x) {
    if ((widgetBackgroundOutline.x < x) &&
        (x < widgetBackgroundOutline.x + widgetBackgroundOutline.width)) {
      if (widgetOutlineBound.lower > x)
        x = widgetOutlineBound.lower;
      if (widgetOutlineBound.upper < x)
        x = widgetOutlineBound.upper;
      widgetOutline.x = x;
      widgetSprite.setPosition((float)x, (float)widgetOutline.y);
    }
  }

  void setMoving(bool isMoving) { moving = isMoving; }
};

#endif  // MINECRAFT_SLIDERWIDGET_HPP
