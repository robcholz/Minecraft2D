//
// Created by robcholz on 6/17/23.
//

#ifndef MINECRAFT_HUNGERBARHUD_HPP
#define MINECRAFT_HUNGERBARHUD_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>
#include <vector>
#include "Hud.hpp"
#include "client/gui/GUI.hpp"
#include "entity/EntityAccess.hpp"
#include "util/math/Random.hpp"
#include "world/WorldAccess.hpp"
#include "world/poi/Coordinate.hpp"

namespace hud {
class HungerBarHud : public GUI, public Hud {
 private:
  using SpriteSmartPtr = std::unique_ptr<sf::Sprite>;

 public:
  explicit HungerBarHud(WorldAccess* worldAccess) {
    this->worldAccess = worldAccess;
    identifier =
        std::make_unique<Identifier>("icons", Identifier::Category::GUI);
    iconAssetPath = identifier->getAbsolutePath();
    // (16,27) (24,35)
    darkOutlineHungerTexture.loadFromFile(iconAssetPath,
                                          sf::IntRect{16, 27, 8, 8});
    // (25,27) (33,35)
    whiteOutlineHungerTexture.loadFromFile(iconAssetPath,
                                           sf::IntRect{25, 27, 8, 8});
    // (53,28) (59,34)
    redHungerTexture.loadFromFile(iconAssetPath, sf::IntRect{53, 28, 6, 6});
    // (62,28) (68,34)
    halfRedHungerTexture.loadFromFile(iconAssetPath, sf::IntRect{62, 28, 6, 6});
    // (71,28) (77,34)
    pinkHungerTexture.loadFromFile(iconAssetPath, sf::IntRect{71, 28, 6, 6});
    // (80,28) (86,34)
    halfPinkHungerTexture.loadFromFile(iconAssetPath,
                                       sf::IntRect{80, 28, 6, 6});
    // (180,28) (186,34)
    emptyHungerTexture.loadFromFile(iconAssetPath, sf::IntRect{180, 28, 6, 6});

    // createCombinedHunger();
    setHungerCapacity(MAX_HUNGER);
    for (auto i = 0; i < MAX_HUNGER; ++i)
      loadHungerOutline(i, &darkOutlineHungerTexture);
    for (auto j = 0; j < MAX_HUNGER; ++j)
      loadHunger(j, &redHungerTexture);
    PLOG_DEBUG << "Loaded HungerBar HUD!";
  }

  void setPosition(HudPixelPosT x, HudPixelPosT z) override {
    Hud::setPosition(x, z);
  }

  void update() override {
    updateData();
    updatePosition();
    // updateHunger();
  }

  void render() override {
    for (const auto& v : hungerBarSpritesContainer) {
      RenderSystem::render(*v.first->get());
      RenderSystem::render(*v.second->get());
    }
  }

 private:
  std::unique_ptr<Identifier> identifier;
  // pair.first is the outline sprite; pair.second is the heart sprite
  std::vector<
      std::pair<std::optional<SpriteSmartPtr>, std::optional<SpriteSmartPtr>>>
      hungerBarSpritesContainer;
  sf::Texture darkOutlineHungerTexture;
  sf::Texture whiteOutlineHungerTexture;
  sf::Texture redHungerTexture;
  sf::Texture pinkHungerTexture;
  sf::Texture halfRedHungerTexture;
  sf::Texture halfPinkHungerTexture;
  sf::Texture redPinkHungerTexture;
  sf::Texture emptyHungerTexture;
  static constexpr int MAX_HUNGER = 10;
  static constexpr int HUNGER_EFFECT_DURATION = 60 * 3;  // seconds
  short last_hunger = 0;
  String iconAssetPath;

  static inline int delay_timer = -1;

  void createCombinedHunger() {
    auto combined_image = pinkHungerTexture.copyToImage();
    for (auto x = 0; x < 4; ++x) {
      for (int y = 0; y < 7; ++y) {
        combined_image.setPixel(
            x, y, halfRedHungerTexture.copyToImage().getPixel(x, y));
      }
    }
    redPinkHungerTexture.loadFromImage(combined_image);
  }

  void setHungerCapacity(int number) {
    for (auto i = 0; i < number; ++i) {
      auto sprite_heart = std::make_unique<sf::Sprite>();
      auto sprite_outline = std::make_unique<sf::Sprite>();
      hungerBarSpritesContainer.emplace_back(std::move(sprite_outline),
                                             std::move(sprite_heart));
    }
  }

  void loadHungerOutline(int number, sf::Texture* outlineTexture) {
    auto sprite_outline = std::make_unique<sf::Sprite>(*outlineTexture);
    setSpriteScale(sprite_outline.get());
    sprite_outline->setPosition(
        (float)getScreenCornerX() +
            (float)number *
                (sprite_outline->getGlobalBounds().width - mapTo(1)),
        (float)getScreenCornerZ());
    hungerBarSpritesContainer[number].first = std::move(sprite_outline);
  }

  void loadHunger(int number, sf::Texture* heartTexture) {
    auto sprite_hunger = std::make_unique<sf::Sprite>(*heartTexture);
    setSpriteScale(sprite_hunger.get());
    auto sprite_outline = hungerBarSpritesContainer[number].first->get();
    sprite_hunger->setPosition(
        (float)getScreenCornerX() +
            (float)number * sprite_outline->getGlobalBounds().width +
            mapTo(1 - number),
        (float)getScreenCornerZ() + mapTo(1));
    hungerBarSpritesContainer[number].second = std::move(sprite_hunger);
  }

  void updateHungerOutlinePos(int number) {
    auto sprite_outline = hungerBarSpritesContainer[number].first->get();
    setSpriteScale(sprite_outline);
    sprite_outline->setPosition(
        (float)getScreenCornerX() +
            (float)number *
                (sprite_outline->getGlobalBounds().width - mapTo(1)),
        (float)getScreenCornerZ());
  }

  void updateHungerPos(int number) {
    auto sprite_outline = hungerBarSpritesContainer[number].first->get();
    auto sprite_hunger = hungerBarSpritesContainer[number].second->get();
    sprite_hunger->setPosition(
        (float)getScreenCornerX() +
            (float)number * sprite_outline->getGlobalBounds().width +
            mapTo(1 - number),
        (float)sprite_outline->getGlobalBounds().top + mapTo(1));
  }

  void updatePosition() override {
    auto pos = convertToPixPos(screenWidth / 2.0f + mapTo(182 / 2 - 9 * 8),
                               screenHeight - mapTo(21 + 9 + 10));
    setPosition(floor(pos.x), floor(pos.y));
    for (auto i = 0; i < MAX_HUNGER; ++i) {
      updateHungerOutlinePos(i);
      updateHungerPos(i);
    }
  }

  void setHungerTexture(int number, sf::Texture* texture) {
    hungerBarSpritesContainer[number].second->get()->setTexture(*texture);
  }

  void setHungerOutlineTexture(sf::Texture* texture) {
    for (int i = 0; i < MAX_HUNGER; ++i) {
      hungerBarSpritesContainer[i].first->get()->setTexture(*texture);
    }
  }

  void shakeHungers() {
    auto move_up_down = [&](int number, bool move) {
      auto sprite_outline = hungerBarSpritesContainer[number].first->get();
      if (move) {
        auto pixelMapVal = RenderSystem::Settings::actualPixelToOnePixel;
        sprite_outline->setPosition(
            sprite_outline->getPosition().x,
            sprite_outline->getPosition().y - pixelMapVal);  // move up
      } else {
        sprite_outline->setPosition(sprite_outline->getPosition().x,
                                    (float)getScreenCornerZ());
      }
      updateHungerPos(number);
    };  // tremble the given hunger
    for (auto i = 0; i < MAX_HUNGER; ++i)
      move_up_down(i, Random::randomBool(1.f / 20));
  }

  void setHungerValue(short hunger) {
    auto hungers_num = hunger / 2;
    auto half_hunger_num = hunger - hungers_num * 2;
    if (hunger != last_hunger || delay_timer == 0)
      for (auto i = 0; i < MAX_HUNGER; ++i)
        setHungerTexture(i, &emptyHungerTexture);
    if (hunger != last_hunger) {
      auto last_hearts_num = last_hunger / 2;
      auto last_half_heart_num = last_hunger - last_hearts_num * 2;
      for (auto j = 0; j < last_hearts_num; ++j)
        setHungerTexture(j, &pinkHungerTexture);
      if (last_half_heart_num != 0 && last_hearts_num < MAX_HUNGER)
        setHungerTexture(last_hearts_num, &halfPinkHungerTexture);
      for (auto k = 0; k < hungers_num; ++k)
        setHungerTexture(k, &redHungerTexture);
      if (half_hunger_num != 0 && hungers_num < MAX_HUNGER)
        setHungerTexture(hungers_num, &redPinkHungerTexture);
      last_hunger = hunger;
    }

    if ((delay_timer / 10) % 2 == 0)
      setHungerOutlineTexture(&darkOutlineHungerTexture);
    else
      setHungerOutlineTexture(&whiteOutlineHungerTexture);

    if (delay_timer == 0) {
      for (auto k = 0; k < hungers_num; ++k)
        setHungerTexture(k, &redHungerTexture);
      if (half_hunger_num != 0 && hungers_num < MAX_HUNGER)
        setHungerTexture(hungers_num, &halfRedHungerTexture);
      setHungerOutlineTexture(&darkOutlineHungerTexture);
    }
  }

  void updateHunger() {
    auto damaged = this->worldAccess->getPlayer()->isDamaged();
    auto hunger = this->worldAccess->getPlayer()->getHealth();
    if (damaged)
      delay_timer = HUNGER_EFFECT_DURATION;
    if (hunger <= worldAccess->getPlayer()->getMaxHealth() / 3)
      shakeHungers();
    setHungerValue(floor(hunger));
    if (delay_timer >= 0)
      delay_timer--;
  }
};
}  // namespace hud

#endif  // MINECRAFT_HUNGERBARHUD_HPP
