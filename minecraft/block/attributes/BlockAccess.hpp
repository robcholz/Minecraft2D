//
// Created by robcholz on 6/12/23.
//

#ifndef MINECRAFT_BLOCKACCESS_HPP
#define MINECRAFT_BLOCKACCESS_HPP

#include "world/poi/Coordinate.hpp"
#include "world/poi/Position.hpp"


namespace sf{class Sprite;}

namespace block {
	class Block;
	class BlockAccess {
	public:
		virtual coordinate::BlockPosition& getPosition() = 0;

		[[nodiscard]]
		virtual struct ID getID() const  = 0;

		[[nodiscard]]
		virtual sf::Sprite* getSprite() const = 0;

		virtual Block* newBlock() = 0;
	};
}

#endif //MINECRAFT_BLOCKACCESS_HPP
