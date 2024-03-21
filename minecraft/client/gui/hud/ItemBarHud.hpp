//
// Created by robcholz on 6/16/23.
//

#ifndef MINECRAFT_ITEMBARHUD_HPP
#define MINECRAFT_ITEMBARHUD_HPP

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
class ItemBarHud : public GUI, public Hud {
 public:
  explicit ItemBarHud(WorldAccess* worldAccess) {
    this->worldAccess = worldAccess;
    identifier =
        std::make_unique<Identifier>("widgets", Identifier::Category::GUI);
    iconAssetPath = identifier->getAbsolutePath();
    itemBarTexture.loadFromFile(iconAssetPath,
                                sf::IntRect{0, 0, 182, 21});  // (0,0) (181,21)
    onFocusedItemTexture.loadFromFile(
        iconAssetPath, sf::IntRect{0, 22, 24, 24});  // (0,22) (24,46)
    scale();
  }

  void update() override {
    updateData();
    updatePosition();
    focus(8);
  }

  void render() override {
    RenderSystem::render(itemBarSprite);
    RenderSystem::render(onFocusedItemSprite);
  }

 private:
  std::unique_ptr<Identifier> identifier;
  sf::Texture itemBarTexture;
  sf::Texture onFocusedItemTexture;
  sf::Sprite itemBarSprite;
  sf::Sprite onFocusedItemSprite;
  String iconAssetPath;

  void scale() {
    itemBarSprite.setTexture(itemBarTexture);
    onFocusedItemSprite.setTexture(onFocusedItemTexture);
    setSpriteScale(&itemBarSprite);
    setSpriteScale(&onFocusedItemSprite);
  }

  void updatePosition() override {
    auto pos = convertToPixPos(
        screenWidth / 2.f - itemBarSprite.getGlobalBounds().width / 2,
        screenHeight - itemBarSprite.getGlobalBounds().height);
    setPosition(floor(pos.x), floor(pos.y));
  }

  void setPosition(HudPixelPosT x, HudPixelPosT z) override {
    Hud::setPosition(x, z);
    itemBarSprite.setPosition((float)getScreenCornerX(),
                              (float)getScreenCornerZ());
  }

  /**
   * @param number
   * @Range 0-8
   */
  void focus(int number) {
    onFocusedItemSprite.setPosition(
        floor(mapTo((number * 20 - 1)) + (float)getScreenCornerX()),
        floor((float)getScreenCornerZ() - mapTo(1)));
  }
};
}  // namespace hud

#endif  // MINECRAFT_ITEMBARHUD_HPP
