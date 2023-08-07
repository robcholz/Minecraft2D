//
// Created by robcholz on 6/11/23.
//

#ifndef MINECRAFT_WORLDACCESS_HPP
#define MINECRAFT_WORLDACCESS_HPP

#include "world/chunk/ChunkStreamAccess.hpp"

namespace entity {
	class PlayerEntity;
}

class WorldAccess {
public:
	virtual entity::PlayerEntity* getPlayer() = 0;

	virtual chunk::ChunkStreamAccess* getChunkStream() = 0;

	virtual sf::View& getView() = 0;
};

#endif //MINECRAFT_WORLDACCESS_HPP
