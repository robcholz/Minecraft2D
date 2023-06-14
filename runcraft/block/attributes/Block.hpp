//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCK_HPP
#define RUNCRAFT_BLOCK_HPP

#include <memory>
#include <string>
#include <map>
#include <limits>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "world/poi/Position.hpp"
#include "block/attributes/BlockTextureLoader.hpp"
#include "client/GameInfo.hpp"
#include "block/attributes/BlockIDLoader.hpp"
#include "BlockState.hpp"
#include "BlockAccess.hpp"
#include "util/Hitbox.hpp"

#define BLOCK_POS_ERROR std::numeric_limits<coordinate::BlockPositionT>::max()

namespace block::blocks { class Blocks; }

namespace block {
	struct ID {
		int serialID{};
		std::string id;
	};

	class Block : public BlockAccess, public HitboxHandler{
	protected:
		virtual Block* newBlock() = 0;

	private:
		friend class block::blocks::Blocks;

		using String = std::string;
		using BlockPosT = coordinate::BlockPositionT;
		using PixelPosT = coordinate::PixelPositonT;

		std::unique_ptr<sf::Sprite> blockSprite;
		std::unique_ptr<BlockPosition> blockPosition;
		std::unique_ptr<BlockTextureLoader> blockTexture;
		std::unique_ptr<BlockState> blockState;

		ID ID;
		Hitbox hitbox;

		void onInitialize() {
			blockSprite->setTexture(*blockTexture->getBlockTextureTile(BlockDirectionType::OUT));
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			blockSprite->setScale((float) zoom / 16.0f, (float) zoom / 16.0f);
			hitbox.setHitbox(getSprite());
		}

	public:
		explicit Block(const String& id) {
			this->ID.id = id;
			this->ID.serialID = BlockIDLoader::getBlockID(id);
			blockTexture = std::make_unique<BlockTextureLoader>(id);
			blockPosition = std::make_unique<BlockPosition>(this, 0, 0);
			blockState = std::make_unique<BlockState>();
			blockSprite = std::make_unique<sf::Sprite>();
			addHitbox(&hitbox);
			onInitialize();
		}

		void setParameter(BlockPosT x, BlockPosT z, BlockDirectionType blockDirection) {
			blockPosition = std::make_unique<BlockPosition>(this, x, z, blockDirection);
			blockState = std::make_unique<BlockState>();
		}

		BlockPosition* getPosition() override { return blockPosition.get(); }

		[[nodiscard]] struct ID getID() const override { return this->ID; }

		[[nodiscard]] sf::Sprite* getSprite() const override { return blockSprite.get(); }

		void onPositionChange() override {
			hitbox.setHitbox(getSprite());
		}

		[[nodiscard]] bool isAir() const { return (this->getID().id == "air_block"); }

		[[nodiscard]] bool isError() const { return (this->getID().id == "error_block"); }

		virtual ~Block() = default;
	};
}


#endif //RUNCRAFT_BLOCK_HPP
