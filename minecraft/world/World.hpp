//
// Created by robcholz on 5/4/23.
//

#ifndef MINECRAFT_WORLD_HPP
#define MINECRAFT_WORLD_HPP

#include <SFML/Graphics/View.hpp>
#include <memory>
#include "client/gui/hud/InGameBarHud.hpp"
#include "client/render/WorldRenderer.hpp"
#include "entity/player/PlayerEntity.hpp"
#include "gen/WorldGeneration.hpp"
#include "world/chunk/WorldChunk.hpp"

class World : public WorldAccess, public SceneAccess {
 public:
  explicit World(MinecraftClientAccess* minecraftClientAccess) {
    this->minecraftClientAccess = minecraftClientAccess;
    player =
        std::make_unique<entity::PlayerEntity>(minecraftClientAccess, this);
    worldGeneration = std::make_unique<WorldGeneration>(20060803);
    worldRenderer = std::make_unique<WorldRenderer>(this);
    chunkManager = std::make_unique<chunk::WorldChunk>(this, 4, 2);
    hud = std::make_unique<hud::InGameBarHud>(minecraftClientAccess, this);

    player->getEntityPosition().set(60, 142);
    chunkManager->setChunkGenerator(
        [this](int chunkPos) { return worldGeneration->getChunk(chunkPos); });
    this->minecraftClientAccess->getSoundManager()->addSound(
        SoundEvents::getInstance().MUSIC_HAL);
  }

  ~World() override = default;

  entity::PlayerEntity* getPlayer() override { return player.get(); }

  chunk::WorldChunk* getChunkManager() override { return chunkManager.get(); }

  sf::View& getView() override { return view; }

  void onUpdate() override {
    chunkManager->update();
    updateCamera();
    player->update();
    hud->update();
  }

  void onRender() override {
    // chunkManager->render();
    worldRenderer->render();
    player->render();
    hud->render();
  }

 private:
  float screenWidth = (float)RenderSystem::getScreenWidth();
  float screenHeight = (float)RenderSystem::getScreenHeight();
  sf::View view{sf::FloatRect{0.f, 0.f, screenWidth, screenHeight}};
  std::unique_ptr<entity::PlayerEntity> player;
  std::unique_ptr<WorldGeneration> worldGeneration;
  std::unique_ptr<WorldRenderer> worldRenderer;
  std::unique_ptr<chunk::WorldChunk> chunkManager;
  std::unique_ptr<hud::InGameBarHud> hud;
  MinecraftClientAccess* minecraftClientAccess = nullptr;

  void updateCamera() {
    view.setCenter(
        (float)player->getEntityPosition().get<coordinate::PixelPos>().x,
        (float)player->getEntityPosition().get<coordinate::PixelPos>().z);
    RenderSystem::setView(view);
  }
};

#endif  // MINECRAFT_WORLD_HPP
