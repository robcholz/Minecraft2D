//
// Created by robcholz on 4/29/23.
//

#ifndef MINECRAFT_SCENEMANAGER_HPP
#define MINECRAFT_SCENEMANAGER_HPP

#include <functional>
#include <map>
#include <string>
#include "SceneAccess.hpp"
#include "client/MinecraftClientAccess.hpp"

class SceneManager {
 private:
  using String = std::string;

 public:
  explicit SceneManager(MinecraftClientAccess* minecraftClientAccess) {
    this->minecraftClientAccess = minecraftClientAccess;
  }

  ~SceneManager() = default;

  SceneManager& setEntry(const String& name) {
    scene = sceneCallableMap[name]();
    sceneName = name;
    return *this;
  }

  SceneManager& addScene(const String& name,
                         std::function<SceneAccess*()> function) {
    sceneCallableMap.insert({name, std::move(function)});
    return *this;
  }

  SceneManager& setPair(const String& first, const String& second) {
    pairMap.insert({first, second});
    return *this;
  }

  void pause() { this->isPaused = true; }

  void resume() { this->isPaused = false; }

  void update() {
    if (!this->isPaused) {
      if (scene->isRunning()) {
        scene->onUpdate();
      } else {
        if (scene->isTerminated()) {
          this->minecraftClientAccess->getSoundManager()
              .clearMusicSoundEventQueue();
          sceneName = pairMap[sceneName];
          delete scene;
          scene = sceneCallableMap[sceneName]();
        }
      }
    }
  }

  void render() {
    if (scene->isRunning()) {
      scene->onRender();
    } else {
      if (scene->isPaused()) {
        scene->onRender();
      }
    }
  }

 private:
  MinecraftClientAccess* minecraftClientAccess = nullptr;
  std::map<String, std::function<SceneAccess*()>> sceneCallableMap;
  std::map<String, String> pairMap;
  SceneAccess* scene = nullptr;
  bool isPaused = false;
  String sceneName;
};

#endif  // MINECRAFT_SCENEMANAGER_HPP
