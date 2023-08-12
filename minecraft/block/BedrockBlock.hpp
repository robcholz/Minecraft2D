//
// Created by robcholz on 7/2/23.
//

#ifndef MINECRAFT_BEDROCKBLOCK_HPP
#define MINECRAFT_BEDROCKBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block {
	class BedrockBlock : public block::Block {
	private:
	public:
		BedrockBlock() : Block("bedrock_block") {
		}

		Block* createObject() override {
			return new BedrockBlock;
		}
	};
}

#endif //MINECRAFT_BEDROCKBLOCK_HPP
