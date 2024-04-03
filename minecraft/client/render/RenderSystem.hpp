//
// Created by robcholz on 3/11/23.
//
#pragma once

#ifndef MINECRAFT_RENDERSYSTEM_HPP
#define MINECRAFT_RENDERSYSTEM_HPP

#include <plog/Log.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <format>
#include <functional>
#include <memory>
#include "util/Path.hpp"
#include "util/math/MathHelper.hpp"

class RenderSystem {
 private:
  using Camera = sf::View;
  using String = std::string;

 public:
  RenderSystem() = delete;

  RenderSystem(const RenderSystem&) = delete;

  ~RenderSystem() = default;

  using ZoomT = short;

  struct Settings {
    /**
     * @range [50,100]
     */
    static constexpr ZoomT zoomSize = 50;
    static constexpr ZoomT pixelToBlock =
        10;  // the screen pixel of one block(16 pixels) // one block take up
             // pixelProportion pixels
    static constexpr ZoomT pixelProportion = (ZoomT)(pixelToBlock * 2);
    static constexpr float actualPixelToOnePixel = (float)pixelProportion / 16;
  };

  static void init(const String& windowName, const String& iconName) {
    screenWidth = (int)((float)sf::VideoMode::getDesktopMode().width * 19 / 32);
    screenHeight =
        (int)((float)sf::VideoMode::getDesktopMode().height * 8 / 15);
    window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight),
                                  windowName);
    camera = window->getDefaultView();
    setMaxFrameRate(60);
    setWindowIcon(iconName);
    PLOG_DEBUG << "Initialize the minecraft main renderer. Parameters: Name: " +
                      windowName + " Size: " + std::to_string(screenWidth) +
                      "*" + std::to_string(screenHeight);
  }

  static void exit() { delete window; }

  static Camera* getCamera() { return &camera; }

  static unsigned int getScreenWidth() { return screenWidth; }

  static unsigned int getScreenHeight() { return screenHeight; }

  [[nodiscard]] static float getFrameRate() { return frameRate; }

  [[nodiscard]] static int getMaxFrameRate() { return maxFrameRate; }

  static void clearWindow(const sf::Color& color = sf::Color(0, 0, 0, 255)) {
    window->clear(color);
  }

  static void setMaxFrameRate(int maxFPS) {
    window->setVerticalSyncEnabled(false);
    maxFrameRate = maxFPS;
    window->setFramerateLimit(maxFPS);
  }

  static void setView(const sf::View& view) { window->setView(view); }

  static void display() { window->display(); }

  static void updateScreenData() {
    // screenWidth = sf::VideoMode::getDesktopMode().width;
    // screenHeight = sf::VideoMode::getDesktopMode().height;
    frameRate = 1.f / frameRateClock.getElapsedTime().asSeconds();
    frameRateClock.restart();
  }

  static sf::RenderWindow* getWindow() { return window; }

  static void render(
      const sf::Drawable& drawable,
      const sf::RenderStates& states = sf::RenderStates::Default) {
    window->draw(drawable, states);
  }

 private:
  static inline Camera camera;
  static inline sf::RenderWindow* window = nullptr;
  static inline sf::Clock frameRateClock;
  static inline int maxFrameRate = 60;
  static inline float frameRate = -1;
  static inline unsigned int screenWidth = -1;
  static inline unsigned int screenHeight = -1;

  static void setWindowIcon(const String& textureName) {
    auto icon = sf::Image();
    String path = Path::texturePath + "blocks/" + textureName + ".png";
    if (!icon.loadFromFile(path)) {
      PLOG_ERROR << "Cannot load application icon from " + path;
      return;
    }
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  }
};

#endif  // MINECRAFT_RENDERSYSTEM_HPP
