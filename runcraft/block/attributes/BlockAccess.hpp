//
// Created by robcholz on 6/12/23.
//

#ifndef RUNCRAFT_BLOCKACCESS_HPP
#define RUNCRAFT_BLOCKACCESS_HPP

#include <SFML/Graphics/Sprite.hpp>
#include "util/MathHelper.hpp"


namespace block {
	class BlockPosition;
	class Block;
	class BlockAccess {
	public:
		virtual BlockPosition* getPosition() = 0;

		[[nodiscard]]
		virtual struct ID getID() const = 0;

		[[nodiscard]]
		virtual sf::Sprite* getSprite() const = 0;

		virtual Block* newBlock() = 0;

		virtual void onPositionChange()=0;
	};
}

#endif //RUNCRAFT_BLOCKACCESS_HPP
