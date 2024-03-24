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
    gameEvents->update();
    keyboard.get().update();
    mouse.get().update();
    sceneManager->update();
    soundManager.update();
  }

  void onRender() { sceneManager->render(); }

 public:
  MinecraftClient()
      : mouse(input::PeripheralsFactory::getMouse()),
        keyboard(input::PeripheralsFactory::getKeyboard()) {
    MinecraftVersion::init();
    RenderSystem::init(MinecraftVersion::getProductionName(),
                       "grass_block_side");
    gameEvents = std::make_unique<SystemEvents>(this);
    sceneManager = std::make_unique<SceneManager>(this);

    gameEvents->onGamePause([this]() { this->sceneManager->pause(); });
    gameEvents->onGameResume([this]() { this->sceneManager->resume(); });
    gameEvents->onWindowResize([]() {
      PLOG_DEBUG << "Yes";
      // sf::FloatRect visibleArea(0, 0, , RenderSystem::getScreenHeight());
      // RenderSystem::getWindow()->setView(sf::View(visibleArea));
    });
    sceneManager->addScene("menu", [this]() { return new Menu(this); })
        .addScene("world", [this]() { return new World(this); })
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

  SoundManager& getSoundManager() override { return soundManager; }

  SceneManager& getSceneManager() override { return *sceneManager; }

  input::mouse::Mouse& getMouse() override { return mouse.get(); }

  input::keyboard::Keyboard& getKeyboard() override { return keyboard.get(); }

 private:
  SoundManager soundManager;
  std::unique_ptr<SceneManager> sceneManager;
  std::unique_ptr<SystemEvents> gameEvents;
  std::reference_wrapper<input::mouse::Mouse> mouse;
  std::reference_wrapper<input::keyboard::Keyboard> keyboard;
};

#endif  // MINECRAFT_MINECRAFTCLIENT_HPP
