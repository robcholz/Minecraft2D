//
// Created by robcholz on 7/2/23.
//

#ifndef RUNCRAFT_BEDROCKBLOCK_HPP
#define RUNCRAFT_BEDROCKBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class BedrockBlock : public block::Block {
	private:
	public:
		BedrockBlock() : Block("bedrock_block") {
		}

		Block* newBlock() override {
			return new BedrockBlock;
		}
	};
}

#endif //RUNCRAFT_BEDROCKBLOCK_HPP
