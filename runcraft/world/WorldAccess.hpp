//
// Created by robcholz on 6/11/23.
//

#ifndef RUNCRAFT_WORLDACCESS_HPP
#define RUNCRAFT_WORLDACCESS_HPP

#include "world/chunk/ChunkStreamAccess.hpp"

class Player;

class WorldAccess{
public:
	virtual Player* getPlayer()=0;
	virtual chunk::ChunkStreamAccess* getChunkStream()=0;
};

#endif //RUNCRAFT_WORLDACCESS_HPP
