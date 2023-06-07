//
// Created by robcholz on 6/6/23.
//
#include "gtest/gtest.h"
#include "block/attributes/BlockIDLoader.hpp"

TEST(BlockIDTest, BasicAssertions) {
	using namespace block;

	EXPECT_EQ(BlockIDLoader::getBlockID("grass_block"),BlockIDLoader::getBlockID("grass_block"));
	EXPECT_EQ(BlockIDLoader::getBlockID("grass_block"),1);
	EXPECT_EQ(BlockIDLoader::getBlockID("air_block"),2);
	EXPECT_EQ(BlockIDLoader::getBlockID("dirt_block"),3);
}
