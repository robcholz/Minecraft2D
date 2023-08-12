//
// Created by robcholz on 4/2/23.
//

#ifndef MINECRAFT_BLOCK_HPP
#define MINECRAFT_BLOCK_HPP

#include <memory>
#include <string>
#include <map>
#include <limits>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "util/Identifier.hpp"
#include "block/attributes/BlockTextureLoader.hpp"
#include "block/attributes/BlockIDLoader.hpp"
#include "BlockState.hpp"
#include "BlockAccess.hpp"
#include "util/Hitbox.hpp"
#include "client/render/TileColor.hpp"


namespace block { class Blocks; }

namespace block {
	struct ID {
		using SerialIDT = uint32_t;
		int serialID = -1;
		std::unique_ptr<Identifier> identifier;
	};

	class Block : public BlockAccess,
	              public HitboxHandler,
	              public sf::Drawable {
	private:
		using String = std::string;
		using BlockPosT = coordinate::BlockPositionT;
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			target.draw(*blockSprite, states);
		}

	public:
		using BlockPtr = Block*;

		explicit Block(const String& id, int luminance = 0) :
				luminance(luminance) {
			this->ID.serialID = BlockIDLoader::getBlockID(id);
			ID.identifier = std::make_unique<Identifier>(id, Identifier::Category::BLOCK);
			blockTexture = std::make_unique<BlockTextureLoader>(*ID.identifier);
			blockPosition = coordinate::BlockPosition(0, 0, coordinate::DirectionType::OUT);
			blockState = std::make_unique<BlockState>();
			blockSprite = std::make_unique<sf::Sprite>();
			addHitbox(&hitbox);
			onInitialize();
		}

		Block(const Block& block) : HitboxHandler(block) {
			this->blockState = std::make_unique<BlockState>(*block.blockState);
			this->blockSprite = std::make_unique<sf::Sprite>(*block.blockSprite);
			blockPosition = block.blockPosition;
			this->blockTexture = std::make_unique<BlockTextureLoader>(*block.blockTexture);
			this->hitbox = block.hitbox;
			this->ID.serialID = block.ID.serialID;
			this->ID.identifier = std::make_unique<Identifier>(*block.ID.identifier);
			this->luminance = block.luminance;
		}

		bool operator==(const Block& block) const {
			return *this->ID.identifier == *block.ID.identifier;
		}

		~Block() override = default;

		void setParameter(BlockPosT x, BlockPosT z, coordinate::DirectionType blockDirection) {
			blockPosition = coordinate::BlockPosition(x, z, blockDirection);
			blockState = std::make_unique<BlockState>();
		}

		void setPosition(const coordinate::BlockPos blockPos) {
			blockPosition.set(blockPos);
			auto pixel_pos = blockPosition.get<coordinate::PixelPos>();
			blockSprite->setPosition((float) pixel_pos.x, (float) pixel_pos.z);
			onPositionChange();
		}

		void setDirection(Direction::DirectionType directionType) {
			getPosition().setDirection(directionType);
			blockSprite->setTexture(*blockTexture->getBlockTextureTile(directionType));
		}

		void setTileColor(const sf::Color& color) {
			blockSprite->setColor(color);
		}

		void setTileColor(uint32_t rgba) {
			blockSprite->setColor(sf::Color(rgba));
		}

		coordinate::BlockPosition& getPosition() override {
			return blockPosition;
		}

		[[nodiscard]]
		Identifier& getID() const override {
			return *this->ID.identifier;
		}

		[[nodiscard]]
		ID::SerialIDT getSerialID() const override {
			return this->ID.serialID;
		}

		Hitbox& getHitbox() override {
			return this->hitbox;
		}

		int getLuminance() override {
			return this->luminance;
		}

		uint8_t getLightLevel() override {
			return TileColor::convertToR(blockSprite->getColor().toInteger()) / 17;
		}

		bool isAir() override {
			return (this->getID().toString() == "minecraft:air_block");
		}

		bool isError() override {
			return (this->getID().toString() == "minecraft:air_block");
		}

		void onPositionChange() {
			hitbox.setHitbox(blockSprite.get());
		}

	private:
		struct ID ID;
		int luminance;
		std::unique_ptr<sf::Sprite> blockSprite;
		coordinate::BlockPosition blockPosition;
		std::unique_ptr<BlockTextureLoader> blockTexture;
		std::unique_ptr<BlockState> blockState;
		Hitbox hitbox{};

		friend class Blocks;

		void onInitialize() {
			blockSprite->setTexture(*blockTexture->getBlockTextureTile(Direction::DirectionType::OUT));
			auto zoom = RenderSystem::Settings::pixelProportion;
			blockSprite->setScale((float) zoom / 16.0f, (float) zoom / 16.0f);
			onPositionChange();
		}
	};
}


#endif //MINECRAFT_BLOCK_HPP
