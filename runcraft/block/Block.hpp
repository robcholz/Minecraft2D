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

class Block {
private:
	std::string id;
	BlockPos blockPos; //Pos,direction
	BlockTextureLoader blockTexture;
	sf::Sprite currentBlockTileSprite;

public:
	explicit Block(const std::string &id) {
		this->id = id;
		blockTexture.loadBlockTiles(id);
		currentBlockTileSprite.setTexture(*blockTexture.getBlockTextureTile(BlockDirectionType::UP));
	}

	void render() { GameInfo.getRender()->render(currentBlockTileSprite); }
};


#endif //RUNCRAFT_BLOCK_HPP
