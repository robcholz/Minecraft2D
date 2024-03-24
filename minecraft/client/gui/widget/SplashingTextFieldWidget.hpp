//
// Created by robcholz on 7/6/23.
//

#ifndef MINECRAFT_SPLASHINGTEXTFIELDWIDGET_HPP
#define MINECRAFT_SPLASHINGTEXTFIELDWIDGET_HPP

#include "Widget.hpp"
#include "client/gui/style/GUIStyle.hpp"
#include "client/gui/text/RichText.hpp"
#include "resource/TranslatableText.hpp"

class SplashingTextFieldWidget : public Widget {
 protected:
  void onRender() override { RenderSystem::render(text); }

 public:
  explicit SplashingTextFieldWidget(const String& id,
                                    int size,
                                    int center,
                                    float angle)
      : Widget("textfield." + id) {
    this->center = center;
    text.setColor(sf::Color::Yellow)
        .setMessage(TranslatableText::getTranslatable(*identifier))
        .setCharacterSize(size)
        .rotate(-20);
    text.setPosition(1000, 300);
  }

  ~SplashingTextFieldWidget() override = default;

  bool stateChanged() = delete;

  bool activated() = delete;

  bool isPressed() = delete;

  bool isClicked() = delete;

 private:
  RichText text{gui_style::getMessageFont()};
  static constexpr float changingSpeed = 0.0035f;
  static constexpr float lower = 1.f;
  static constexpr float upper = 1.05f;
  int center;
  float scale = 1.f;

  void executeCallbackFunc() override {}

  void onUpdate() override {
    static bool flag;
    if (scale >= upper)
      flag = true;
    if (scale <= lower)
      flag = false;
    if (flag)
      scale -= changingSpeed;
    else
      scale += changingSpeed;
    text.setScale(scale, scale);
  }
};

#endif  // MINECRAFT_SPLASHINGTEXTFIELDWIDGET_HPP
