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

#define registerBlock(block) _registerBlock(std::make_shared<block>())

namespace block::blocks {
	class Blocks {
	private:
		using String = std::string;
		using BlockPtr = Block::BlockPtr;
	public:
		BlockPtr AIR_BLOCK;
		BlockPtr BEDROCK_BLOCK;
		BlockPtr BIRCH_LOG_BLOCK;
		BlockPtr COAL_ORE_BLOCK;
		BlockPtr COBBLESTONE_BLOCK;
		BlockPtr DIAMOND_BLOCK;
		BlockPtr DIRT_BLOCK;
		BlockPtr ERROR_BLOCK;
		BlockPtr GRASS_BLOCK;
		BlockPtr IRON_BLOCK;
		BlockPtr STONE_BLOCK;
		BlockPtr DEEPSLATE_BLOCK;
		BlockPtr ANDESITE_BLOCK;
		BlockPtr DIORITE_BLOCK;
		BlockPtr GRANITE_BLOCK;

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
		std::unordered_map<String, Block::BlockPtr> blocksIDRegistry;
		std::unordered_map<int, Block::BlockPtr> blocksSerialIDRegistry;

		explicit Blocks() {
			AIR_BLOCK = registerBlock(AirBlock);
			BEDROCK_BLOCK = registerBlock(BedrockBlock);
			BIRCH_LOG_BLOCK = registerBlock(BirchLogBlock);
			COAL_ORE_BLOCK = registerBlock(CoalOreBlock);
			COBBLESTONE_BLOCK = registerBlock(CobblestoneBlock);
			DIAMOND_BLOCK = registerBlock(DiamondBlock);
			DIRT_BLOCK = registerBlock(DirtBlock);
			ERROR_BLOCK = registerBlock(ErrorBlock);
			GRASS_BLOCK = registerBlock(GrassBlock);
			IRON_BLOCK = registerBlock(IronBlock);
			STONE_BLOCK = registerBlock(StoneBlock);
			DEEPSLATE_BLOCK = registerBlock(DeepslateBlock);
			ANDESITE_BLOCK = registerBlock(AndesiteBlock);
			DIORITE_BLOCK = registerBlock(DioriteBlock);
			GRANITE_BLOCK = registerBlock(GraniteBlock);
		}

	private:
		const Block::BlockPtr& _registerBlock(const Block::BlockPtr& block) {
			blocksIDRegistry.insert({block->getID().id, block});
			blocksSerialIDRegistry.insert({block->getID().serialID, block});
			return block;
		}
	};
}

#endif //RUNCRAFT_BLOCKS_HPP
