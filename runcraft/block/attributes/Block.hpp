//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCK_HPP
#define RUNCRAFT_BLOCK_HPP

#include <memory>
#include <string>
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "sound/BlockSoundGroup.hpp"
#include "block/attributes/BlockPosition.hpp"
#include "block/attributes/BlockTextureLoader.hpp"
#include "client/GameInfo.hpp"
#include "block/attributes/BlockIDLoader.hpp"
#include "BlockState.hpp"

namespace block::blocks { class Blocks; }

namespace block {
	class Block {
	protected:
		virtual Block* getBlockInstance() = 0;

	private:
		friend class block::blocks::Blocks;

		using String = std::string;
		using BlockPosT = coordinate::BlockPositionT;
		using PixelPosT = coordinate::PixelPositonT;

		struct ID {
			int serialID{};
			std::string id;
		} ID;

		std::unique_ptr<sf::Sprite> blockSprite;
		std::unique_ptr<BlockPosition> blockPosition;
		std::unique_ptr<BlockTextureLoader> blockTexture;
		std::unique_ptr<BlockState> blockState;

		void onInitialize() {
			blockSprite->setTexture(*blockTexture->getBlockTextureTile(BlockDirectionType::OUT));
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			blockSprite->setScale((float) zoom / 16.0f, (float) zoom / 16.0f);
		}

	public:
		explicit Block(const String& id) {
			this->ID.id = id;
			this->ID.serialID = BlockIDLoader::getBlockID(id);

			blockTexture = std::make_unique<BlockTextureLoader>(id);
			blockPosition = std::make_unique<BlockPosition>(0, 0);
			blockState = std::make_unique<BlockState>();
			blockSprite = std::make_unique<sf::Sprite>();
			onInitialize();
		}

		void setParameter(BlockPosT x, BlockPosT z, BlockDirectionType blockDirection) {
			blockPosition = std::make_unique<BlockPosition>(x, z, blockDirection);
			blockState = std::make_unique<BlockState>();
		}

		static coordinate::PixelPositonT convertToPixelPos(BlockPosT blockPos) {
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			return zoom * blockPos;
		}

		static BlockPosT convertToBlockPos(PixelPosT pixelPos) {
			auto zoom = GameInfo.getConstExternalData()->windowState.pixelProportion;
			return (int) (pixelPos / (double) (zoom));
		}

		[[nodiscard]] struct ID getID() const { return this->ID; }

		[[nodiscard]] BlockPosition* getPositionPtr() const { return blockPosition.get(); }

		[[nodiscard]] sf::Sprite* getSpritePtr() const { return blockSprite.get(); }

		virtual ~Block() = default;
	};
}


#endif //RUNCRAFT_BLOCK_HPP
