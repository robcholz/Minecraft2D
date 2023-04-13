//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCK_HPP
#define RUNCRAFT_BLOCK_HPP

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include "sound/BlockSoundGroup.hpp"
#include "BlockPos.hpp"

class Block{
private:
	std::string id;
	BlockPos blockPos;
	sf::Texture texture;
public:
	explicit Block(std::string id, ){

	}
};


#endif //RUNCRAFT_BLOCK_HPP
