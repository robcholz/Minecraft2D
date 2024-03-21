//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_TEXTUREDBUTTONWIDGET_HPP
#define MINECRAFT_TEXTUREDBUTTONWIDGET_HPP

#include "Widget.hpp"

class TexturedButtonWidget : public Widget {
 public:
  explicit TexturedButtonWidget(int width = 32,
                                int height = 32,
                                bool visible = true,
                                int x = 0,
                                int y = 0)
      : Widget("", visible, {0, 106, 20, 20}, {0, 126, 20, 20}) {
    widgetSprite.setScale((float)width / 8, (float)height / 8);
    auto width_ = (int)widgetSprite.getGlobalBounds().width;
    auto height_ = (int)widgetSprite.getGlobalBounds().height;
    setOutline(&widgetOutline, widgetSprite, x, y, width_, height_);
    widgetSprite.setPosition((float)widgetOutline.x, (float)widgetOutline.y);
  }

  ~TexturedButtonWidget() override = default;

 private:
  CallbackFunc execFuncPtr = nullptr;
  RichText message;
};

#endif  // MINECRAFT_TEXTUREDBUTTONWIDGET_HPP
