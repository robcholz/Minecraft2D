//
// Created by robcholz on 6/16/23.
//

#ifndef MINECRAFT_EXPERIENCEBARHUD_HPP
#define MINECRAFT_EXPERIENCEBARHUD_HPP

#include <plog/Log.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>
#include "Hud.hpp"
#include "client/gui/GUI.hpp"
#include "entity/EntityAccess.hpp"
#include "world/WorldAccess.hpp"

namespace hud {

class ExperienceBarHud : public GUI, public Hud {
 public:
  explicit ExperienceBarHud(WorldAccess* worldAccess) {
    this->worldAccess = worldAccess;
    identifier =
        std::make_unique<Identifier>("icons", Identifier::Category::GUI);
    iconAssetPath = identifier->getAbsolutePath();
    // empty experience bar (0,64) (182,69)
    loadSprite(&emptyExperienceBarTexture, &emptyExperienceBarSprite,
               sf::IntRect{0, 64, 182, 5});
    // full experience bar (0,69) (182,74)
    loadSprite(&fullExperienceBarTexture, &fullExperienceBarSprite,
               sf::IntRect{0, 69, 182, 5});
    // empty experience slot (1,70) (9,72)
    // loadSprite(&leftExperienceSlotTexture, &leftExperienceSlotSprite,
    // sf::IntRect{0, 0, 0, 0});
    PLOG_DEBUG << "Loaded ExperienceBar HUD!";
  }

  void update() override {
    updateData();
    updatePosition();
  }

  void render() override { RenderSystem::render(emptyExperienceBarSprite); }

 private:
  std::unique_ptr<Identifier> identifier;
  std::vector<std::unique_ptr<sf::Sprite>> experienceBarContainer;
  sf::Texture emptyExperienceBarTexture;
  sf::Texture fullExperienceBarTexture;
  sf::Texture emptyExperienceSlotTexture;

  sf::Sprite emptyExperienceBarSprite;
  sf::Sprite fullExperienceBarSprite;
  sf::Sprite emptyExperienceSlotSprite;

  String iconAssetPath;

  void setPosition(HudPixelPosT x, HudPixelPosT z) override {
    Hud::setPosition(x, z);
    emptyExperienceBarSprite.setPosition((float)getScreenCornerX(),
                                         (float)getScreenCornerZ());
  }

  void loadSprite(sf::Texture* texture,
                  sf::Sprite* sprite,
                  sf::IntRect intRect) {
    texture->loadFromFile(iconAssetPath, intRect);
    sprite->setTexture(*texture);
    setSpriteScale(sprite);
  }

  void updatePosition() override {
    auto pos = convertToPixPos(screenWidth / 2.0f - mapTo((182 / 2)),
                               screenHeight - mapTo((21 + 8)));
    setPosition(floor(pos.x), floor(pos.y));
  }
};
}  // namespace hud

#endif  // MINECRAFT_EXPERIENCEBARHUD_HPP
