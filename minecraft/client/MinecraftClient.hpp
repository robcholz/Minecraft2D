//
// Created by robcholz on 6/15/23.
//

#ifndef MINECRAFT_MINECRAFTCLIENT_HPP
#define MINECRAFT_MINECRAFTCLIENT_HPP

#include "Menu.hpp"
#include "MinecraftClientAccess.hpp"
#include "MinecraftVersion.hpp"
#include "client/scene/SceneManager.hpp"
#include "sound/SoundManager.hpp"
#include "util/GameLogger.hpp"
#include "world/World.hpp"

class MinecraftClient : public MinecraftClientAccess {
 protected:
  void onUpdate() {
    SystemEvents::getInstance().update();
    keyboard.get().update();
    mouse.get().update();
    sceneManager->update();
    soundManager->update();
  }

  void onRender() { sceneManager->render(); }

 public:
  MinecraftClient()
      : mouse(input::PeripheralsFactory::getMouse()),
        keyboard(input::PeripheralsFactory::getKeyboard()) {
    MinecraftVersion::init();
    RenderSystem::init(MinecraftVersion::getProductionName(),
                       "grass_block_side");
    sceneManager = std::make_unique<SceneManager>(this);
    soundManager = std::make_unique<SoundManager>();

    SystemEvents::getInstance().onGamePause([this]() { this->sceneManager->pause(); });
    SystemEvents::getInstance().onGameResume([this]() { this->sceneManager->resume(); });
    SystemEvents::getInstance().onWindowResize([]() {
      PLOG_DEBUG << "Yes";
      // sf::FloatRect visibleArea(0, 0, , RenderSystem::getScreenHeight());
      // RenderSystem::getWindow()->setView(sf::View(visibleArea));
    });
    sceneManager
        ->addScene("menu", [this]() { return std::make_unique<Menu>(this); })
        .addScene("world", [this]() { return std::make_unique<World>(this); })
        .setPair("menu", "world")
        .setEntry("menu");
  }

  ~MinecraftClient() { RenderSystem::exit(); };

  void run() {
    while (RenderSystem::getWindow()->isOpen()) {
      RenderSystem::clearWindow(sf::Color(120, 167, 255));
      RenderSystem::updateScreenData();
      onUpdate();
      onRender();
      RenderSystem::display();
    }
  }

  SoundManager& getSoundManager() override { return *soundManager; }

  SceneManager& getSceneManager() override { return *sceneManager; }

  input::mouse::Mouse& getMouse() override { return mouse.get(); }

  input::keyboard::Keyboard& getKeyboard() override { return keyboard.get(); }

 private:
  std::unique_ptr<SoundManager> soundManager;
  std::unique_ptr<SceneManager> sceneManager;
  std::reference_wrapper<input::mouse::Mouse> mouse;
  std::reference_wrapper<input::keyboard::Keyboard> keyboard;
};

#endif  // MINECRAFT_MINECRAFTCLIENT_HPP
