//
// Created by robcholz on 6/26/23.
//
#include <gtest/gtest.h>
#include "util/Identifier.hpp"

TEST(IdentifierTest,BasicAssertion){
	std::string id1="runcraft:block.air_block";
	std::string id2="block.dirt_block";
	std::string id3="block.blockstates.dirt_block";
	Identifier identifier_1(id1,Identifier::Category::TEXTURE);
	Identifier identifier_2(id2,Identifier::Category::TEXTURE);
	Identifier identifier_3(id3,Identifier::Category::TEXTURE);
	ASSERT_EQ(identifier_1.getNamespace(),"runcraft");
	ASSERT_EQ(identifier_1.getRelativePath(), "block/air_block");
	ASSERT_EQ(identifier_3.getRelativePath(),"block/blockstates/dirt_block");
	ASSERT_EQ(identifier_1.getNamespace(),identifier_2.getNamespace());
	ASSERT_NE(identifier_1.getRelativePath(), identifier_2.getRelativePath());
}