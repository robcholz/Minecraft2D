//
// Created by robcholz on 6/12/23.
//

#ifndef RUNCRAFT_ERRORBLOCK_HPP
#define RUNCRAFT_ERRORBLOCK_HPP

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

#endif //RUNCRAFT_ERRORBLOCK_HPP
