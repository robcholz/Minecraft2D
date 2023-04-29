//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCK_HPP
#define RUNCRAFT_BLOCK_HPP

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "sound/BlockSoundGroup.hpp"
#include "BlockPos.hpp"
#include "BlockTextureLoader.hpp"
#include "client/GameInfo.hpp"

namespace block {

	class Block {
	private:
		std::string id;
		//BlockPos blockPos; //Pos,direction
		sf::Sprite currentBlockSprite;

		inline static BlockTextureLoader *blockTextureInstance;

		static BlockTextureLoader *getTextureInstance(const std::string &id) {
			if (!blockTextureInstance) {
				blockTextureInstance = new BlockTextureLoader(id);
				return blockTextureInstance;
			} else return blockTextureInstance;
		}

		static void freeTexture() { if (!blockTextureInstance) delete blockTextureInstance; }

	public:
		explicit Block(const std::string &id) {
			this->id = id;
			currentBlockSprite.setTexture(*getTextureInstance(id)->getBlockTextureTile(OUT));
		}

		~Block() {
			freeTexture();
		}

		sf::Sprite *getCurrentSprite() { return &currentBlockSprite; }
	};
}


#endif //RUNCRAFT_BLOCK_HPP
