//
// Created by robcholz on 8/20/23.
//

#ifndef MINECRAFT_2D_LIGHTINGPROVIDERACCESS_HPP
#define MINECRAFT_2D_LIGHTINGPROVIDERACCESS_HPP


#include <cstdint>
#include "world/poi/Coordinate.hpp"


class LightingProviderAccess {
public:
	virtual void updateLightSource(const coordinate::BlockPos& blockPosition, uint8_t lightLevel) = 0;
};

#endif //MINECRAFT_2D_LIGHTINGPROVIDERACCESS_HPP
