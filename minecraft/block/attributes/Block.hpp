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


namespace block::blocks { class Blocks; }

namespace block {
	struct ID {
		int serialID = -1;
		std::string tempID;
		Identifier* identifier = nullptr;
	};

	class Block : public BlockAccess, public HitboxHandler {
	private:
		using String = std::string;
		using BlockPosT = coordinate::BlockPositionT;

		void onInitialize() {
			blockSprite->setTexture(*blockTexture->getBlockTextureTile(Direction::DirectionType::OUT));
			auto zoom = RenderSystem::Settings::pixelProportion;
			blockSprite->setScale((float) zoom / 16.0f, (float) zoom / 16.0f);
			hitbox.setHitbox(getSprite());
		}

	public:
		using BlockPtr = std::shared_ptr<Block>;

		explicit Block(const String& id) {
			this->ID.tempID = id;
			this->ID.serialID = BlockIDLoader::getBlockID(id);
			identifier = std::make_unique<Identifier>(id, Identifier::Category::BLOCK);
			ID.identifier = identifier.get();
			blockTexture = std::make_unique<BlockTextureLoader>(*identifier);
			blockPosition = coordinate::BlockPosition(0, 0, coordinate::DirectionType::OUT);
			blockState = std::make_unique<BlockState>();
			blockSprite = std::make_unique<sf::Sprite>();
			addHitbox(&hitbox);
			onInitialize();
		}

		Block(const Block& block) : HitboxHandler(block) {
			this->identifier = std::make_unique<Identifier>(*block.identifier);
			this->blockState = std::make_unique<BlockState>(*block.blockState);
			this->blockSprite = std::make_unique<sf::Sprite>(*block.blockSprite);
			blockPosition = block.blockPosition;
			this->blockTexture = std::make_unique<BlockTextureLoader>(*block.blockTexture);
			this->hitbox = block.hitbox;
			this->ID = block.ID;
		}

		void setParameter(BlockPosT x, BlockPosT z, coordinate::DirectionType blockDirection) {
			blockPosition = coordinate::BlockPosition(x, z, blockDirection);
			blockState = std::make_unique<BlockState>();
		}

		void setPosition(const coordinate::BlockPos blockPos){
			blockPosition.set(blockPos);
			auto pixel_pos=blockPosition.get<coordinate::PixelPos>();
			blockSprite->setPosition((float)pixel_pos.x,(float)pixel_pos.z);
			onPositionChange();
		}

		coordinate::BlockPosition& getPosition() override {
			return blockPosition;
		}

		[[nodiscard]]
		struct ID getID() const override { return this->ID; }

		[[nodiscard]]
		sf::Sprite* getSprite() const override { return blockSprite.get(); }

		void onPositionChange() {
			hitbox.setHitbox(getSprite());
		}

		[[nodiscard]]
		bool isAir() const { return (this->getID().tempID == "air_block"); }

		[[nodiscard]]
		bool isError() const { return (this->getID().tempID == "error_block"); }

		virtual ~Block() = default;

	private:
		std::unique_ptr<Identifier> identifier;
		std::unique_ptr<sf::Sprite> blockSprite;
		coordinate::BlockPosition blockPosition;
		std::unique_ptr<BlockTextureLoader> blockTexture;
		std::unique_ptr<BlockState> blockState;
		Hitbox hitbox{};
		struct ID ID;

		friend class Blocks;
	};
}


#endif //MINECRAFT_BLOCK_HPP
