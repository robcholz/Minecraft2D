//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCK_HPP
#define RUNCRAFT_BLOCK_HPP

//TODO
//TODO

#include "sound/BlockSoundGroup.hpp"

class Block{

};

class Settings {
private:
	bool collidable;
	BlockSoundGroup soundGroup;
	//ToIntFunction <BlockState> luminance;
	float resistance;
	float hardness;
	bool randomTicks;
	float slipperiness;
	float velocityMultiplier;
	float jumpVelocityMultiplier;
	//Identifier lootTableId;
	//bool opaque;
	//Boolean isAir;
	//TypedContextPredicate<EntityType< ?>> allowsSpawningPredicate;
	//ContextPredicate solidBlockPredicate;
	//ContextPredicate suffocationPredicate;
	//ContextPredicate blockVisionPredicate;

	//ContextPredicate postProcessPredicate;

	//ContextPredicate emissiveLightingPredicate;

	//Boolean dynamicBounds;


	Settings() {

	}

public:
};

#endif //RUNCRAFT_BLOCK_HPP
