//
// Created by robcholz on 4/2/23.
//

#ifndef RUNCRAFT_BLOCKS_HPP
#define RUNCRAFT_BLOCKS_HPP

#include <memory>
#include "block/AirBlock.hpp"
#include "block/DirtBlock.hpp"
#include "block/GrassBlock.hpp"
#include "block/IronBlock.hpp"

namespace block::blocks {
#define RegisterBlock(x) registerBlock(std::make_shared<x>())
	class Blocks {
	private:
		using String = std::string;
		using BlockSmartPtr = std::shared_ptr<Block>;
		static inline std::map<String, BlockSmartPtr> blocksIDRegistry;
		static inline std::map<int, BlockSmartPtr> blocksSerialIDRegistry;

		explicit Blocks() {
			RegisterBlock(AirBlock);
			RegisterBlock(DirtBlock);
			RegisterBlock(GrassBlock);
			RegisterBlock(IronBlock);
		}
	protected:
		static void registerBlock(const BlockSmartPtr& block) {
			blocksIDRegistry.insert({block->getID().id, block});
			blocksSerialIDRegistry.insert({block->getID().serialID, block});
		}
	public:
		static std::shared_ptr<Blocks> getInstance(){
			static std::shared_ptr<Blocks> instance(new Blocks);
			return instance;
		}

		static Block* getBlockInstance(const String &id) {
			return blocksIDRegistry[id]->getBlockInstance();
		}

		static Block* getBlockInstance(int serialID) {
			return blocksSerialIDRegistry[serialID]->getBlockInstance();
		}

		~Blocks() = default;
	};
}

#endif //RUNCRAFT_BLOCKS_HPP
