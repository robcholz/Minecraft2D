//
// Created by robcholz on 6/12/23.
//

#ifndef MINECRAFT_BLOCKACCESS_HPP
#define MINECRAFT_BLOCKACCESS_HPP

#include "world/poi/Coordinate.hpp"
#include "world/poi/Position.hpp"
#include "util/RegistryAccess.hpp"


namespace sf { class Sprite; }
class Hitbox;

namespace block {
	class Block;

	class BlockAccess : public utils::RegistryAccess<Block> {
	public:
		virtual coordinate::BlockPosition& getPosition() = 0;

		[[nodiscard]]
		virtual Identifier& getID() const = 0;

		[[nodiscard]]
		virtual unsigned int getSerialID() const = 0;

		virtual Hitbox& getHitbox() = 0;

		virtual int getLuminance() = 0;

		virtual uint8_t getLightLevel() = 0;

		virtual bool isAir() = 0;

		virtual bool isError() = 0;
	};
}

#endif //MINECRAFT_BLOCKACCESS_HPP
