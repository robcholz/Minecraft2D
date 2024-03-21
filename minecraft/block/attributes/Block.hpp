//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_BLOCK_HPP
#define MINECRAFT_BLOCK_HPP

#include <mempool/MemoryPool.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include "BlockAccess.hpp"
#include "BlockState.hpp"
#include "block/attributes/BlockIDLoader.hpp"
#include "block/attributes/TextureLoader.hpp"
#include "client/render/TileColor.hpp"
#include "resource/PooledSprite.hpp"
#include "util/Hitbox.hpp"
#include "util/Identifier.hpp"

namespace block {
class Blocks;
}

namespace block {
struct ID {
  using SerialIDT = uint32_t;
  int serialID = -1;
  std::unique_ptr<Identifier> identifier;
};

class Block : public BlockAccess, public HitboxHandler, public sf::Drawable {
 private:
  using String = std::string;
  using BlockPosT = coordinate::BlockPositionT;

 protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(*blockSprite, states);
  }

 public:
  using BlockPtr = Block*;

  explicit Block(const String& id, int luminance = 0) : luminance(luminance) {
    this->blockID.serialID = BlockIDLoader::getBlockID(id);
    blockID.identifier =
        std::make_unique<Identifier>(id, Identifier::Category::BLOCK);
    blockPosition =
        coordinate::BlockPosition(0, 0, coordinate::DirectionType::OUT);
    blockState = std::make_unique<BlockState>();
    blockSprite = std::make_unique<resource::Sprite>();
    addHitbox(&hitbox);
    onInitialize();
  }

  Block(const Block& block) : HitboxHandler(block) {
    this->blockState = std::make_unique<BlockState>(*block.blockState);
    this->blockSprite = std::make_unique<resource::Sprite>(*block.blockSprite);
    blockPosition = block.blockPosition;
    this->hitbox = block.hitbox;
    this->blockID.serialID = block.blockID.serialID;
    this->blockID.identifier =
        std::make_unique<Identifier>(*block.blockID.identifier);
    this->luminance = block.luminance;
  }

  bool operator==(const Block& block) const {
    return *this->blockID.identifier == *block.blockID.identifier;
  }

  ~Block() override = default;

  void setParameter(BlockPosT x,
                    BlockPosT z,
                    coordinate::DirectionType blockDirection) {
    blockPosition = coordinate::BlockPosition(x, z, blockDirection);
    blockState = std::make_unique<BlockState>();
  }

  void setPosition(const coordinate::BlockPos blockPos) {
    blockPosition.set(blockPos);
    auto pixel_pos = blockPosition.get<coordinate::PixelPos>();
    blockSprite->setPosition((float)pixel_pos.x, (float)pixel_pos.z);
    onPositionChange();
  }

  void setDirection(Direction::DirectionType directionType) {
    getPosition().setDirection(directionType);
    blockSprite->setTexture(*TextureLoader::getInstance().getBlockTextureTile(
        getID(), directionType));
  }

  void setTileColor(const sf::Color& color) { blockSprite->setColor(color); }

  void setTileColor(uint32_t rgba) { blockSprite->setColor(sf::Color(rgba)); }

  coordinate::BlockPosition& getPosition() override { return blockPosition; }

  [[nodiscard]] Identifier& getID() const override {
    return *this->blockID.identifier;
  }

  [[nodiscard]] ID::SerialIDT getSerialID() const override {
    return this->blockID.serialID;
  }

  Hitbox& getHitbox() override { return this->hitbox; }

  int getLuminance() override { return this->luminance; }

  uint8_t getLightLevel() override {
    return TileColor::convertToR(blockSprite->getColor().toInteger()) / 17;
  }

  bool isAir() override {
    return (this->getID().toString() == "minecraft:air_block");
  }

  bool isError() override {
    return (this->getID().toString() == "minecraft:air_block");
  }

  void onPositionChange() { hitbox.setHitbox(blockSprite.get()); }

 private:
  struct ID blockID;
  int luminance;
  std::unique_ptr<resource::Sprite> blockSprite;
  coordinate::BlockPosition blockPosition;
  std::unique_ptr<BlockState> blockState;
  Hitbox hitbox{};

  friend class Blocks;

  void onInitialize() {
    blockSprite->setTexture(*TextureLoader::getInstance().getBlockTextureTile(
        getID(), coordinate::DirectionType::OUT));
    auto zoom = RenderSystem::Settings::pixelProportion;
    blockSprite->setScale((float)zoom / 16.0f, (float)zoom / 16.0f);
    onPositionChange();
  }
};
}  // namespace block

#endif  // MINECRAFT_BLOCK_HPP
