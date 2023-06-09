//
// Created by robcholz on 5/4/23.
//

#ifndef RUNCRAFT_CHUNKSTREAM_HPP
#define RUNCRAFT_CHUNKSTREAM_HPP

#include <utility>
#include <vector>
#include <list>
#include <memory>
#include "Chunk.hpp"
#include "world/storage/SaveHelper.hpp"
#include "util/Utils.hpp"

namespace chunk {

	class ChunkStream {
	protected:
		using DistanceT = unsigned short;
	private:
		using ChunkPtr = Chunk*;
		using String = std::string;
		using ChunkPosT = coordinate::ChunkPositionT;
		std::map<ChunkPosT, ChunkPtr> chunkRenderingList; // chunks that will be rendered
		std::map<ChunkPosT, ChunkPtr> chunkSimulationList; // chunks that will be updated
		std::list<ChunkPosT> chunkDeletingList;
		DistanceT renderDistance = 0, simulationDistance = 0;
		Intervali renderInterval{}, simulationInterval{};
		std::unique_ptr<SaveHelper> saveHelper;
		std::unique_ptr<FileHelper> regionFileHelper;
		std::function<Chunk*(ChunkPosT)> chunkGenerator = nullptr;
		Player* player = nullptr;
		ChunkPosT playerChunkPos = 0;

		/**/
		std::set<ChunkPosT> cachedChunks;

		void updateInterval() {
			auto x_player = player->getPosition()->getX();
			playerChunkPos = Chunk::convertToChunkPos((int)x_player);
			renderInterval.lower = playerChunkPos - renderDistance;
			renderInterval.upper = playerChunkPos + renderDistance;
			simulationInterval.lower = playerChunkPos - simulationDistance;
			simulationInterval.upper = playerChunkPos + simulationDistance;
		}

		void indexRegions() {
			for (const auto& filepath: *regionFileHelper->getFilesInDirectory()) {
				auto filename = filepath.substr(filepath.find_last_of("/\\") + 1);
				auto second_dot_occur = utils::nthOccurrence(filename, ".", 2);
				auto third_dot_occur = utils::nthOccurrence(filename, ".", 3);
				coordinate::ChunkPositionT chunkPos = std::stoi(filename.substr(second_dot_occur + 1, third_dot_occur - second_dot_occur));
				cachedChunks.insert(chunkPos);
			}
		}

		void addToIndexedRegions(ChunkPosT chunkPos) {
			cachedChunks.insert(chunkPos);
		}

		Chunk* getChunk(ChunkPosT chunkPos) {
			Chunk* chunk;
			if (cachedChunks.count(chunkPos))
				chunk = saveHelper->loadChunk(chunkPos);
			else {
				chunk = chunkGenerator(chunkPos);
				addToIndexedRegions(chunkPos);
			}
			return chunk;
		}

		void addToSimulationChunks(ChunkPosT chunkPos) {
			auto chunk_1 = getChunk(chunkPos);
			chunkSimulationList.insert({chunkPos, chunk_1});
		}

		void removeFromSimulationChunks(ChunkPosT chunkPos) {
			auto chunk = chunkSimulationList[chunkPos];
			saveHelper->saveChunk(chunk);
			chunkDeletingList.push_front(chunkPos);
		}

		void removeRedundantChunks(){
			for (auto chunkPos:chunkDeletingList) {
				delete chunkSimulationList[chunkPos];
				chunkSimulationList.erase(chunkPos);
			}
			chunkDeletingList.clear();
		}

		/**
		 * @Usage Update active queue(list)
		 */
		void updateSimulationChunks() {
			for (auto v: chunkSimulationList) {
				auto chunkPos = v.first;
				if (!(simulationInterval.lower <= chunkPos && chunkPos <= simulationInterval.upper)) {
					removeFromSimulationChunks(chunkPos); // chunk has been unloaded
				}
			}
			for (auto chunkPos = simulationInterval.lower; chunkPos <= simulationInterval.upper; chunkPos++)
				if (!chunkSimulationList.contains(chunkPos))
					addToSimulationChunks(chunkPos); // load chunks
		}

		/**
		 * @Usage Update render queue(list)
		 */
		void updateRenderingChunks() {
			//TODO Should also depend on the zoom size
			for(auto v:chunkDeletingList){
				chunkRenderingList.erase(v);
			}
			for (auto v: chunkSimulationList) {
				auto chunkPos = v.first;
				auto chunk = v.second;
				if (!(renderInterval.lower <= chunkPos && chunkPos <= renderInterval.upper)) {
					chunkRenderingList.erase(chunkPos);
				}// chunk has been de-rendered
				else {
					if (!chunkRenderingList.contains(chunkPos)) {
						chunkRenderingList.insert({chunkPos, chunk});
					} // not found
				}// chunk should be rendered
			}
		}

		void onUpdate() {
			updateInterval();
			updateSimulationChunks();
			updateRenderingChunks();
			removeRedundantChunks();
		}

	public:
		explicit ChunkStream(Player* playerPtr,DistanceT simulationDistance, DistanceT renderDistance) {
			player = playerPtr;
			saveHelper = std::make_unique<SaveHelper>("TestChunkSave");
			regionFileHelper = std::make_unique<FileHelper>(saveHelper->getDirectory() + "/region");
			setSimulationDistance(simulationDistance);
			setRenderDistance(renderDistance);
			indexRegions();
		}

		void setRenderDistance(DistanceT distance) {
			if (distance >= simulationDistance)
				renderDistance = simulationDistance;
			else
				renderDistance = distance;
		}

		void setSimulationDistance(DistanceT distance) {
			simulationDistance = distance;
		}

		void setChunkGenerator(std::function<Chunk*(ChunkPosT)> function) {
			chunkGenerator = std::move(function);
		}

		void update() {
			onUpdate();
			for (const auto& v: chunkSimulationList) {
				v.second->update();
			}
		}

		void render() {
			for (const auto& v: chunkRenderingList) {
				v.second->render();
			}
		}

	};
}

#endif //RUNCRAFT_CHUNKSTREAM_HPP
