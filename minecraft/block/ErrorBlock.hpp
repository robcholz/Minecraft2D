//
// Created by robcholz on 6/12/23.
//

#ifndef MINECRAFT_ERRORBLOCK_HPP
#define MINECRAFT_ERRORBLOCK_HPP

#include "block/attributes/Block.hpp"

namespace block::blocks {
	class ErrorBlock : public block::Block {
	private:
	public:
		ErrorBlock() : Block("error_block") {
			enableHitbox(false);
		}

		Block* newBlock() override {
			return new ErrorBlock;
		}
	};
}

#endif //MINECRAFT_ERRORBLOCK_HPP
