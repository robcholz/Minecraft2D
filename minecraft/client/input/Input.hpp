//
// Created by robcholz on 5/19/23.
//
#pragma once

#ifndef MINECRAFT_INPUT_HPP
#define MINECRAFT_INPUT_HPP

#include <SFML/Window/Event.hpp>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "events/SystemEvents.hpp"

namespace input {
enum class MouseKeyType {
  Left = sf::Mouse::Button::Left,          ///< The left mouse button
  Right = sf::Mouse::Button::Right,        ///< The right mouse button
  Middle = sf::Mouse::Button::Middle,      ///< The middle (wheel) mouse button
  XButton1 = sf::Mouse::Button::XButton1,  ///< The first extra mouse button
  XButton2 = sf::Mouse::Button::XButton2,  ///< The second extra mouse button
  VerticalWheel,                           ///< The vertical mouse wheel
  HorizontalWheel                          ///< The horizontal mouse wheel
};
using KeyboardKeyType = sf::Keyboard::Key;

namespace peripherals {
template <class Key, typename KeyType>
class Peripherals {
 protected:
  std::map<KeyType, std::unique_ptr<Key>> loadedPeripheralsList;

 public:
  virtual ~Peripherals() = default;

  Key& addKey(KeyType keyType) {
    if (!loadedPeripheralsList.contains(keyType))
      loadedPeripheralsList.insert({keyType, std::make_unique<Key>(keyType)});
    return *loadedPeripheralsList[keyType];
  }

  std::optional<std::reference_wrapper<Key>> getKey(KeyType keyType) {
    if (loadedPeripheralsList.contains(keyType))
      return *loadedPeripheralsList[keyType];
    return std::nullopt;
  }

  virtual void onUpdate() {
    for (auto& v : loadedPeripheralsList) {
      v.second->update();
    }
  }

  void update() { onUpdate(); }
};

template <typename KeyType>
class Key {
 protected:
  bool pressed = false;
  KeyType toggleKey{};

 public:
  explicit Key(KeyType key) { toggleKey = key; }

  [[nodiscard]] KeyType getKey() const { return toggleKey; }

  virtual void update() = 0;

  ~Key() = default;

  [[nodiscard]] bool isPressed() const { return pressed; }
};
}  // namespace peripherals

namespace keyboard {

class KeyboardKey : public peripherals::Key<KeyboardKeyType> {
 public:
  explicit KeyboardKey(KeyboardKeyType key) : Key<KeyboardKeyType>(key) {}

  void update() override { pressed = sf::Keyboard::isKeyPressed(toggleKey); }

  ~KeyboardKey() = default;
};

class Keyboard : public peripherals::Peripherals<KeyboardKey, KeyboardKeyType> {
 public:
  explicit Keyboard() = default;

  ~Keyboard() override = default;
};
}  // namespace keyboard

namespace mouse {
class MouseKey : public peripherals::Key<MouseKeyType> {
 private:
  struct Wheel {
    float delta;
    float change;
  } wheel{};

 public:
  explicit MouseKey(MouseKeyType key) : Key<MouseKeyType>(key) {}

  [[nodiscard]] bool isAnalog() const {
    return getKey() == MouseKeyType::HorizontalWheel ||
           getKey() == MouseKeyType::VerticalWheel;
  }

  [[nodiscard]] Wheel getWheel() const { return wheel; }

  void update() override {
    if (isAnalog()) {
      if (SystemEvents::getInstance().getEvent().type == sf::Event::MouseWheelScrolled)
        wheel.delta = SystemEvents::getInstance().getEvent().mouseWheelScroll.delta;
    } else {
      pressed =
          sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(toggleKey));
    }
  }
};

class Mouse : public input::peripherals::Peripherals<MouseKey, MouseKeyType> {
 private:
  sf::Vector2i position{};

 public:
  explicit Mouse() = default;

  void onUpdate() override {
    peripherals::Peripherals<MouseKey, MouseKeyType>::onUpdate();
    position = sf::Mouse::getPosition(*RenderSystem::getWindow());
  }

  sf::Vector2i& getPosition() { return position; }

  ~Mouse() override = default;
};
}  // namespace mouse

class PeripheralsFactory {
 public:
  PeripheralsFactory() = delete;

  PeripheralsFactory(const PeripheralsFactory&) = delete;

  ~PeripheralsFactory() = delete;

  static keyboard::Keyboard& getKeyboard() {
    static keyboard::Keyboard keyboard;
    return keyboard;
  }

  static mouse::Mouse& getMouse() {
    static mouse::Mouse mouse;
    return mouse;
  }
};

class KeyboardObserver {
 private:
  std::vector<std::reference_wrapper<keyboard::KeyboardKey>> hotKeyList;
  std::reference_wrapper<keyboard::Keyboard> keyboard;

 public:
  KeyboardObserver() : keyboard(PeripheralsFactory::getKeyboard()) {}

  KeyboardObserver& attachKey(KeyboardKeyType keyType) {
    auto key = keyboard.get().getKey(keyType);
    if (!key.has_value())
      hotKeyList.emplace_back(keyboard.get().addKey(keyType));
    else
      hotKeyList.push_back(key.value());
    return *this;
  }

  [[nodiscard]] bool isActivated() const {
    return std::all_of(hotKeyList.cbegin(), hotKeyList.cend(),
                       [](auto key) { return key.get().isPressed(); });
  }

  ~KeyboardObserver() = default;
};

class MouseObserver {
 private:
  std::vector<std::reference_wrapper<mouse::MouseKey>> hotKeyList;
  std::reference_wrapper<mouse::Mouse> mouse;

 public:
  MouseObserver() : mouse(PeripheralsFactory::getMouse()) {}

  MouseObserver& attachKey(MouseKeyType keyType) {
    auto key = mouse.get().getKey(keyType);
    if (!key.has_value())
      hotKeyList.emplace_back(mouse.get().addKey(keyType));
    else
      hotKeyList.push_back(key.value());
    return *this;
  }

  [[nodiscard]] bool isActivated() const {
    return std::all_of(hotKeyList.cbegin(), hotKeyList.cend(),
                       [](auto key) { return key.get().isPressed(); });
  }

  sf::Vector2i getPosition() { return mouse.get().getPosition(); }

  ~MouseObserver() = default;
};
}  // namespace input

#endif  // MINECRAFT_INPUT_HPP
