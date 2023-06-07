//
// Created by robcholz on 6/6/23.
//

#ifndef RUNCRAFT_BLOCKTEXTURELOADERTEST_HPP
#define RUNCRAFT_BLOCKTEXTURELOADERTEST_HPP

#include "block/attributes/BlockTextureLoader.hpp"
#include "gtest/gtest.h"

TEST(BlockIDTest, BasicAssertions) {
	using namespace block;
	BlockTextureLoader block_texture_loader{"grass_block"};
	BlockTextureLoader block_texture_loader_1{"grass_block"};
	BlockTextureLoader block_texture_loader_2{"air_block"};
	BlockTextureLoader block_texture_loader_3{"iron_block"};
	BlockTextureLoader block_texture_loader_4{"dirt_block"};
	BlockTextureLoader block_texture_loader_5{"air_block"};

	EXPECT_EQ(block_texture_loader.getBlockTextureTile(BlockDirectionType::UP), block_texture_loader_1.getBlockTextureTile(BlockDirectionType::UP));
	ASSERT_FALSE(block_texture_loader.getBlockTextureTile(BlockDirectionType::UP) == block_texture_loader_3.getBlockTextureTile(BlockDirectionType::UP));
}

#endif //RUNCRAFT_BLOCKTEXTURELOADERTEST_HPP
