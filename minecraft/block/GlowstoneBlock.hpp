//
// Created by robcholz on 8/11/23.
//

#ifndef MINECRAFT_2D_GLOWSTONEBLOCK_HPP
#define MINECRAFT_2D_GLOWSTONEBLOCK_HPP


#include "block/attributes/Block.hpp"


namespace block {
	class GlowstoneBlock : public block::Block {
	private:
	public:
		explicit GlowstoneBlock() : Block("glowstone_block", 15) {
			enableHitbox(true);
		}

		Block* createObject() override {
			return new GlowstoneBlock;
		}
	};
}

#endif //MINECRAFT_2D_GLOWSTONEBLOCK_HPP
