//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCKS_HPP
#define RUNCRAFT_BLOCKS_HPP

#include <memory>
#include "block/attributes/Block.hpp"
#include "block/AirBlock.hpp"
#include "block/DirtBlock.hpp"
#include "block/GrassBlock.hpp"
#include "block/IronBlock.hpp"
#include "block/BirchLogBlock.hpp"
#include "block/CoalOreBlock.hpp"
#include "block/CobblestoneBlock.hpp"
#include "block/DiamondBlock.hpp"
#include "block/ErrorBlock.hpp"
#include "block/StoneBlock.hpp"
#include "block/BedrockBlock.hpp"
#include "block/DeepslateBlock.hpp"
#include "block/AndesiteBlock.hpp"
#include "block/DioriteBlock.hpp"
#include "block/GraniteBlock.hpp"

#define RegisterBlock(block) registerBlock(std::make_shared<block>())

namespace block::blocks {
	class Blocks {
	private:
		using String = std::string;
		using BlockSmartPtr = std::shared_ptr<Block>;

		explicit Blocks() {
			RegisterBlock(AirBlock);
			RegisterBlock(BedrockBlock);
			RegisterBlock(BirchLogBlock);
			RegisterBlock(CoalOreBlock);
			RegisterBlock(CobblestoneBlock);
			RegisterBlock(DiamondBlock);
			RegisterBlock(DirtBlock);
			RegisterBlock(ErrorBlock);
			RegisterBlock(GrassBlock);
			RegisterBlock(IronBlock);
			RegisterBlock(StoneBlock);
			RegisterBlock(DeepslateBlock);
			RegisterBlock(AndesiteBlock);
			RegisterBlock(DioriteBlock);
			RegisterBlock(GraniteBlock);
		}

	protected:
		void registerBlock(const BlockSmartPtr& block) {
			blocksIDRegistry.insert({block->getID().id, block});
			blocksSerialIDRegistry.insert({block->getID().serialID, block});
		}

	public:
		static std::shared_ptr<Blocks> getInstance() {
			static std::shared_ptr<Blocks> instance(new Blocks);
			return instance;
		}

		Block* newBlock(const String& id) {
			return blocksIDRegistry[id]->newBlock();
		}

		Block* newBlock(int serialID) {
			return blocksSerialIDRegistry[serialID]->newBlock();
		}

		Block* getBlockInstance(const String& id) {
			return blocksIDRegistry[id].get();
		}

		Block* getBlockInstance(int serialID) {
			return blocksSerialIDRegistry[serialID].get();
		}

		~Blocks() = default;

	private:
		std::unordered_map<String, BlockSmartPtr> blocksIDRegistry;
		std::unordered_map<int, BlockSmartPtr> blocksSerialIDRegistry;
	};
}

#endif //RUNCRAFT_BLOCKS_HPP
