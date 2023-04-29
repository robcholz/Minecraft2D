//
// Created by robcholz on 4/14/23.
//

#include <iostream>
#include "resource/FileHelper.hpp"
#include "gtest/gtest.h"

TEST(BlockTest, BasicAssertions) {
	EXPECT_STRNE("hello", "world");
	EXPECT_EQ(7 * 6, 42);
}
