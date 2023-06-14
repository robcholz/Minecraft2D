//
// Created by robcholz on 5/4/23.
//

#ifndef RUNCRAFT_CHUNKSTREAM_HPP
#define RUNCRAFT_CHUNKSTREAM_HPP

#include <utility>
#include <vector>
#include <list>
#include <memory>
#include <set>
#include "Chunk.hpp"
#include "world/storage/SaveHelper.hpp"
#include "util/Utils.hpp"
#include "world/WorldAccess.hpp"
#include "ChunkStreamAccess.hpp"

namespace chunk {
	class ChunkStream : public ChunkStreamAccess{
	protected:
		using DistanceT = unsigned short;
	private:
		using ChunkPtr = Chunk*;
		using String = std::string;
		using ChunkPosT = coordinate::ChunkPositionT;
		using BlockPosT = coordinate::BlockPositionT;
		using BlockCoordinate = coordinate::Coordinate<BlockPosT>;
		std::map<ChunkPosT, ChunkPtr> chunkRenderingMap; // chunks that will be rendered
		std::map<ChunkPosT, ChunkPtr> chunkSimulationMap; // chunks that will be updated
		std::list<ChunkPosT> chunkDeletingList;
		DistanceT renderDistance = 0, simulationDistance = 0;
		Intervali renderInterval{}, simulationInterval{};
		std::unique_ptr<SaveHelper> saveHelper;
		std::unique_ptr<FileHelper> regionFileHelper;
		std::function<Chunk*(ChunkPosT)> chunkGenerator = nullptr;
		WorldAccess* worldAccess= nullptr;
		ChunkPosT playerChunkPos = 0;

		/**/
		std::set<ChunkPosT> cachedChunks;

		void updateInterval(){
			auto x_player = worldAccess->getPlayer()->getEntityPosition().getPixelPosition().getCoordinate().x;
			playerChunkPos = Chunk::convertToChunkPos(x_player);
			renderInterval.lower = playerChunkPos - renderDistance;
			renderInterval.upper = playerChunkPos + renderDistance;
			simulationInterval.lower = playerChunkPos - simulationDistance;
			simulationInterval.upper = playerChunkPos + simulationDistance;
		}

		void indexRegions(){
			for (const auto& filepath: *regionFileHelper->getFilesInDirectory()) {
				auto filename = filepath.substr(filepath.find_last_of("/\\") + 1);
				auto second_dot_occur = utils::nthOccurrence(filename, ".", 2);
				auto third_dot_occur = utils::nthOccurrence(filename, ".", 3);
				coordinate::ChunkPositionT chunkPos = std::stoi(filename.substr(second_dot_occur + 1, third_dot_occur - second_dot_occur));
				cachedChunks.insert(chunkPos);
			}
		}

		void addToIndexedRegions(ChunkPosT chunkPos){
			cachedChunks.insert(chunkPos);
		}

		Chunk* loadChunk(ChunkPosT chunkPos){
			Chunk* chunk;
			if (cachedChunks.count(chunkPos))
				chunk = saveHelper->loadChunk(chunkPos);
			else {
				chunk = chunkGenerator(chunkPos);
				addToIndexedRegions(chunkPos);
			}
			return chunk;
		}

		void addToSimulationChunks(ChunkPosT chunkPos){
			auto chunk = loadChunk(chunkPos);
			chunkSimulationMap.insert({chunkPos, chunk});
		}

		void removeFromSimulationChunks(ChunkPosT chunkPos){
			auto chunk = chunkSimulationMap[chunkPos];
			saveHelper->saveChunk(chunk);
			chunkDeletingList.push_front(chunkPos);
		}

		void removeRedundantChunks(){
			for (auto chunkPos:chunkDeletingList) {
				delete chunkSimulationMap[chunkPos];
				chunkSimulationMap.erase(chunkPos);
			}
			chunkDeletingList.clear();
		}

		void updateSimulationChunks(){
			for (auto v: chunkSimulationMap) {
				auto chunkPos = v.first;
				if (!(simulationInterval.lower <= chunkPos && chunkPos <= simulationInterval.upper)) {
					removeFromSimulationChunks(chunkPos); // chunk has been unloaded
				}
			}
			for (auto chunkPos = simulationInterval.lower; chunkPos <= simulationInterval.upper; chunkPos++)
				if (!chunkSimulationMap.contains(chunkPos))
					addToSimulationChunks(chunkPos); // load chunks
		}

		void updateRenderingChunks(){
			for(auto v:chunkDeletingList){
				chunkRenderingMap.erase(v);
			}
			for (auto v: chunkSimulationMap) {
				auto chunkPos = v.first;
				auto chunk = v.second;
				if (!(renderInterval.lower <= chunkPos && chunkPos <= renderInterval.upper)) {
					chunkRenderingMap.erase(chunkPos);
				}// chunk has been de-rendered
				else {
					if (!chunkRenderingMap.contains(chunkPos)) {
						chunkRenderingMap.insert({chunkPos, chunk});
					} // not found
				}// chunk should be rendered
			}
		}

		void onUpdate(){
			updateInterval();
			updateSimulationChunks();
			updateRenderingChunks();
			removeRedundantChunks();
		}

	public:
		explicit ChunkStream(WorldAccess* worldAccess, DistanceT simulationDistance, DistanceT renderDistance){
			this->worldAccess=worldAccess;
			saveHelper = std::make_unique<SaveHelper>("TestChunkSave");
			regionFileHelper = std::make_unique<FileHelper>(saveHelper->getDirectory() + "/region");
			setSimulationDistance(simulationDistance);
			setRenderDistance(renderDistance);
			indexRegions();
		}

		~ChunkStream(){
			for(auto v:chunkSimulationMap)
				saveHelper->saveChunk(v.second);
		}

		void setRenderDistance(DistanceT distance){
			if (distance >= simulationDistance)
				renderDistance = simulationDistance;
			else
				renderDistance = distance;
		}

		void setSimulationDistance(DistanceT distance){
			simulationDistance = distance;
		}

		void setChunkGenerator(std::function<Chunk*(ChunkPosT)> function){
			chunkGenerator = std::move(function);
		}

		Chunk* getChunk(ChunkPosT chunkPos){
			if(chunkSimulationMap.contains(chunkPos))
				return chunkSimulationMap[chunkPos];
			return nullptr;
		}

		block::Block* getBlock(BlockCoordinate blockCoordinate) override {
			// convert to chunk position and search in stream
			// locate the block pointer in a specific chunk
			auto chunkSettings = Chunk::toChunkSettings(blockCoordinate);
			auto chunk = this->getChunk(chunkSettings.chunkPos);
			if (chunk != nullptr)
				return chunk->getBlockWithBoundaryCheck(chunkSettings.chunkBlockPos);
			PLOG_ERROR << "Given block doesn't exist in the loaded chunks: ChunkPosition: "<<chunkSettings.chunkPos;
			return block::blocks::Blocks::getInstance()->getBlockInstance("error_block");
		}

		void update(){
			onUpdate();
			for (const auto& v: chunkSimulationMap) {
				v.second->update();
			}
		}

		void render(){
			for (const auto& v: chunkRenderingMap) {
				v.second->render();
			}
		}

	};
}

#endif //RUNCRAFT_CHUNKSTREAM_HPP
