//
// Created by robcholz on 4/14/23.
//

#include <iostream>
#include "resource/FileHelper.hpp"
#include "gtest/gtest.h"

TEST(ResourceTest, BasicAssertions) {
	EXPECT_STRNE("hello", "worldAccess");
	EXPECT_EQ(7 * 6, 42);
}

//FileHelper::transferJsonPathToFilePath("runcraft:block/grass_block_side")
